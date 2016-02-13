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

#ifndef WRITER_APPROACHLEGWRITER_H_
#define WRITER_APPROACHLEGWRITER_H_

#include "fs/writer/ap/legbasewriter.h"
#include "fs/bgl/ap/approachleg.h"

namespace atools {
namespace fs {
namespace writer {

class ApproachLegWriter :
  public atools::fs::writer::LegBaseWriter
{
public:
  ApproachLegWriter(atools::sql::SqlDatabase& db, atools::fs::writer::DataWriter& dataWriter)
    : LegBaseWriter(db, dataWriter, "approach_leg")
  {
  }

  virtual ~ApproachLegWriter()
  {
  }

protected:
  virtual void writeObject(const atools::fs::bgl::ApproachLeg *type) override;

};

} // namespace writer
} // namespace fs
} // namespace atools

#endif /* WRITER_APPROACHLEGWRITER_H_ */