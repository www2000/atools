/*****************************************************************************
* Copyright 2015-2017 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "fs/xp/xpfixwriter.h"

#include "fs/xp/xpairportindex.h"
#include "fs/xp/xpconstants.h"
#include "fs/progresshandler.h"
#include "fs/common/magdecreader.h"
#include "geo/pos.h"

#include "sql/sqlutil.h"

using atools::sql::SqlQuery;
using atools::sql::SqlUtil;

namespace atools {
namespace fs {
namespace xp {

// lat lon
// ("28.000708333", "-83.423330556", "KNOST", "ENRT", "K7")
enum FieldIndex
{
  LATY = 0,
  LONX = 1,
  IDENT = 2,
  AIRPORT = 3,
  REGION = 4
};

XpFixWriter::XpFixWriter(atools::sql::SqlDatabase& sqlDb, XpAirportIndex *xpAirportIndex,
                         const NavDatabaseOptions& opts, ProgressHandler *progressHandler)
  : XpWriter(sqlDb, opts, progressHandler), airportIndex(xpAirportIndex)
{
  initQueries();
}

XpFixWriter::~XpFixWriter()
{
  deInitQueries();
}

void XpFixWriter::write(const QStringList& line, const XpWriterContext& context)
{
  atools::geo::Pos pos(line.at(LONX).toFloat(), line.at(LATY).toFloat());

  insertWaypointQuery->bindValue(":waypoint_id", ++curFixId);
  insertWaypointQuery->bindValue(":file_id", context.curFileId);
  insertWaypointQuery->bindValue(":ident", line.at(IDENT));
  insertWaypointQuery->bindValue(":airport_id", airportIndex->getAirportId(line.at(AIRPORT)));
  insertWaypointQuery->bindValue(":region", line.at(REGION)); // ZZ for no region
  insertWaypointQuery->bindValue(":type", "WN");
  insertWaypointQuery->bindValue(":num_victor_airway", 0);
  insertWaypointQuery->bindValue(":num_jet_airway", 0);
  insertWaypointQuery->bindValue(":mag_var", context.magDecReader->getMagVar(pos));
  insertWaypointQuery->bindValue(":lonx", pos.getLonX());
  insertWaypointQuery->bindValue(":laty", pos.getLatY());
  insertWaypointQuery->exec();

  progress->incNumWaypoints();
}

void XpFixWriter::finish(const XpWriterContext& context)
{
  Q_UNUSED(context);
}

void XpFixWriter::initQueries()
{
  deInitQueries();

  SqlUtil util(&db);

  insertWaypointQuery = new SqlQuery(db);
  insertWaypointQuery->prepare(util.buildInsertStatement("waypoint", QString(), {"nav_id"}));
}

void XpFixWriter::deInitQueries()
{
  delete insertWaypointQuery;
  insertWaypointQuery = nullptr;
}

} // namespace xp
} // namespace fs
} // namespace atools