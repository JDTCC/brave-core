/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>
#include <memory>

#include "ads_client.h"
#include "catalog_state.h"
#include "callback_handler.h"

namespace rewards_ads {
class AdsImpl;
}  // namespace rewards_ads

namespace state {

class Catalog: public ads::CallbackHandler {
 public:
  explicit Catalog(ads::AdsClient* ads_client);
  ~Catalog();

  bool LoadJson(const std::string& json);  // Deserialize

  std::shared_ptr<CATALOG_STATE> GetCatalogState() const;

 private:
  ads::AdsClient* ads_client_;  // NOT OWNED

  std::shared_ptr<CATALOG_STATE> catalog_state_;
};

}  // namespace state
