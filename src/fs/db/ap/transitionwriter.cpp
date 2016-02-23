/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
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

#include "fs/db/datawriter.h"
#include "fs/bgl/util.h"
#include "fs/db/ap/transitionwriter.h"
#include "fs/bglreaderoptions.h"
#include "fs/db/ap/approachwriter.h"
#include "fs/bgl/ap/approachtypes.h"
#include "fs/db/ap/airportwriter.h"
#include "fs/db/ap/transitionlegwriter.h"

namespace atools {
namespace fs {
namespace db {

using atools::fs::bgl::Transition;
using atools::sql::SqlQuery;

void TransitionWriter::writeObject(const Transition *type)
{
  if(getOptions().isVerbose())
    qDebug() << "Writing Transition for airport "
             << getDataWriter().getAirportWriter()->getCurrentAirportIdent();

  bind(":transition_id", getNextId());
  bind(":approach_id", getDataWriter().getApproachWriter()->getCurrentId());
  bind(":type", Transition::transitionTypeToStr(type->getType()));
  bindNullInt(":fix_nav_id");
  bind(":fix_type", Transition::transitionFixTypeToStr(type->getTransFixType()));
  bind(":fix_ident", type->getTransFixIdent());
  bind(":fix_region", type->getFixRegion());
  bind(":fix_airport_ident", type->getFixAirportIdent());
  bind(":altitude", bgl::util::meterToFeet(type->getAltitude(), 1));

  if(type->getType() == bgl::ap::APPR_TRANS_DME)
  {
    bind(":dme_ident", type->getDmeIdent());
    bind(":dme_region", type->getDmeRegion());
    bind(":dme_airport_ident", type->getDmeAirportIdent());
    bind(":dme_radial", type->getDmeRadial());
    bind(":dme_distance", bgl::util::meterToNm(type->getDmeDist()));
  }
  else
  {
    bindNullString(":dme_ident");
    bindNullString(":dme_region");
    bindNullString(":dme_airport_ident");
    bindNullInt(":dme_radial");
    bindNullInt(":dme_distance");
  }
  executeStatement();

  getDataWriter().getApproachTransLegWriter()->write(type->getLegs());
}

} // namespace writer
} // namespace fs
} // namespace atools