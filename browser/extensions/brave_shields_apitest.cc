/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/path_service.h"
#include "brave/common/brave_paths.h"
#include "brave/common/extensions/extension_constants.h"
#include "brave/common/pref_names.h"
#include "brave/components/brave_wallet/browser/buildflags/buildflags.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "components/prefs/pref_service.h"
#include "extensions/test/result_catcher.h"

namespace extensions {
namespace {

class BraveShieldsExtensionApiTest : public ExtensionApiTest {
 public:
  void SetUp() override {
    brave::RegisterPathProvider();
    base::PathService::Get(brave::DIR_TEST_DATA, &extension_dir_);
    extension_dir_ = extension_dir_.AppendASCII("extensions/api_test");
    ExtensionApiTest::SetUp();
  }
  void TearDown() override {
    ExtensionApiTest::TearDown();
  }
  PrefService* GetPrefs() { return browser()->profile()->GetPrefs(); }
  base::FilePath extension_dir_;
};

IN_PROC_BROWSER_TEST_F(BraveShieldsExtensionApiTest, BraveExtensionHasAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("braveShields"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(BraveShieldsExtensionApiTest,
    NotBraveExtensionHasNoAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("notBraveShields"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

#if BUILDFLAG(BRAVE_WALLET_ENABLED)
IN_PROC_BROWSER_TEST_F(BraveShieldsExtensionApiTest,
    BraveExtensionWithWalletHasAccess) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("braveShieldsWithWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(BraveShieldsExtensionApiTest,
    BraveWalletAPIAvailable) {
  std::unique_ptr<base::Environment> env(base::Environment::Create());
  env->SetVar("BRAVE_INFURA_PROJECT_ID", "test-project-id");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("braveWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testBasics()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(BraveShieldsExtensionApiTest,
    BraveWalletAPIKnownValuesTest) {
  GetPrefs()->SetString(kBraveWalletAES256GCMSivNonce, "yJngKDr5nCGYz7EM");
  GetPrefs()->SetString(kBraveWalletEncryptedSeed,
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("braveWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testKnownSeedValuesEndToEnd()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(BraveShieldsExtensionApiTest,
    BraveWalletAPINotAvailable) {
  ResultCatcher catcher;
  const Extension* extension =
    LoadExtension(extension_dir_.AppendASCII("notBraveWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}
#endif

}  // namespace
}  // namespace extensions
