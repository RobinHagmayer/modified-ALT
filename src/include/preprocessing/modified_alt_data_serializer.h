#pragma once

#include <string>

#include "preprocessing/data_types.h"

namespace modified_alt {
namespace preprocessing {

void SerializeModifiedAltData(const std::string &path,
                              const ModifiedAltData &data);

ModifiedAltData DeserializeModifiedAltData(const std::string &path);

}  // namespace preprocessing
}  // namespace modified_alt
