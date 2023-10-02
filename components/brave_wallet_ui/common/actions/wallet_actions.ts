/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { WalletActions } from '../slices/wallet.slice'

// We must re-export actions here until we remove all imports of this file
export const {
  accountsChanged,
  activeOriginChanged,
  addAccount,
  addFavoriteApp,
  addSitePermission,
  addUserAsset,
  addUserAssetError,
  autoLockMinutesChanged,
  backedUp,
  defaultBaseCryptocurrencyChanged,
  defaultBaseCurrencyChanged,
  defaultCurrenciesUpdated,
  defaultEthereumWalletChanged,
  defaultEthereumWalletUpdated,
  defaultSolanaWalletChanged,
  defaultSolanaWalletUpdated,
  expandWalletNetworks,
  getAllNetworks,
  getAllTokensList,
  getCoinMarkets,
  hasIncorrectPassword,
  initialize,
  initialized,
  keyringCreated,
  keyringReset,
  walletRestored,
  locked,
  lockWallet,
  portfolioTimelineUpdated,
  refreshAll,
  refreshAccountInfo,
  refreshBalancesAndPriceHistory,
  refreshNetworksAndTokens,
  removeFavoriteApp,
  removeSitePermission,
  removeUserAsset,
  selectPortfolioTimeline,
  setAllTokensList,
  setAssetAutoDiscoveryCompleted,
  setCoinMarkets,
  setGasEstimates,
  setHasFeeEstimatesError,
  setMetaMaskInstalled,
  selectOnRampAssetId,
  setPasswordAttempts,
  setSelectedAccountFilterItem,
  setSelectedAssetFilterItem,
  setSelectedGroupAssetsByItem,
  setSelectedNetworkFilter,
  setSitePermissions,
  setSolFeeEstimates,
  setUserAssetVisible,
  setVisibleTokensInfo,
  unlocked,
  unlockWallet,
  updateUserAsset,
  setHidePortfolioGraph,
  setHidePortfolioBalances,
  setRemovedFungibleTokenIds,
  setRemovedNonFungibleTokenIds,
  setHidePortfolioNFTsTab,
  setRemovedNonFungibleTokens,
  setFilteredOutPortfolioNetworkKeys,
  setFilteredOutPortfolioAccountAddresses,
  setHidePortfolioSmallBalances,
  setShowNetworkLogoOnNfts,
  setIsRefreshingNetworksAndTokens,
  updateAccountName,
  removeAccount,
  importAccount,
  setImportAccountError,
  importAccountFromJson
} = WalletActions
