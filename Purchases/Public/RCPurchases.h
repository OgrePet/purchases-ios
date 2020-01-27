//
//  RCPurchases.h
//  Purchases
//
//  Created by RevenueCat.
//  Copyright © 2019 RevenueCat. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "RCOffering.h"
#import "RCOfferings.h"

@class SKProduct, SKPayment, SKPaymentTransaction, SKPaymentDiscount, SKProductDiscount, RCPurchaserInfo, RCIntroEligibility;
@protocol RCPurchasesDelegate;

NS_ASSUME_NONNULL_BEGIN

/**
 Completion block for calls that send back a `PurchaserInfo`
 */
typedef void (^RCReceivePurchaserInfoBlock)(RCPurchaserInfo * _Nullable, NSError * _Nullable) NS_SWIFT_NAME(Purchases.ReceivePurchaserInfoBlock);

/**
 Completion block for `-[RCPurchases checkTrialOrIntroductoryPriceEligibility:completionBlock:]`
 */
typedef void (^RCReceiveIntroEligibilityBlock)(NSDictionary<NSString *, RCIntroEligibility *> *) NS_SWIFT_NAME(Purchases.ReceiveIntroEligibilityBlock);

/**
 Completion block for `-[RCPurchases offeringsWithCompletionBlock:]`
 */
typedef void (^RCReceiveOfferingsBlock)(RCOfferings * _Nullable, NSError * _Nullable) NS_SWIFT_NAME(Purchases.ReceiveOfferingsBlock);

/**
 Completion block for `-[RCPurchases productsWithIdentifiers:completionBlock:]`
 */
typedef void (^RCReceiveProductsBlock)(NSArray<SKProduct *> *) NS_SWIFT_NAME(Purchases.ReceiveProductsBlock);

/**
 Completion block for `-[RCPurchases purchaseProduct:withCompletionBlock:]`
 */
typedef void (^RCPurchaseCompletedBlock)(SKPaymentTransaction * _Nullable, RCPurchaserInfo * _Nullable, NSError * _Nullable, BOOL userCancelled) NS_SWIFT_NAME(Purchases.PurchaseCompletedBlock);

/**
 Deferred block for `purchases:shouldPurchasePromoProduct:defermentBlock:`
 */
typedef void (^RCDeferredPromotionalPurchaseBlock)(RCPurchaseCompletedBlock);

/**
Deferred block for `-[RCPurchases paymentDiscountForProductDiscount:product:completion:]`
*/
API_AVAILABLE(ios(12.2), macos(10.14.4))
typedef void (^RCPaymentDiscountBlock)(SKPaymentDiscount * _Nullable, NSError * _Nullable) NS_SWIFT_NAME(Purchases.PaymentDiscountBlock);

/**
 Enum of supported attribution networks
 */
typedef NS_ENUM(NSInteger, RCAttributionNetwork) {
    /**
     Apple's search ads
     */
    RCAttributionNetworkAppleSearchAds = 0,
    /**
     Adjust https://www.adjust.com/
     */
    RCAttributionNetworkAdjust,
    /**
     AppsFlyer https://www.appsflyer.com/
     */
    RCAttributionNetworkAppsFlyer,
    /**
     Branch https://www.branch.io/
     */
    RCAttributionNetworkBranch,
    /**
     Tenjin https://www.tenjin.io/
     */
    RCAttributionNetworkTenjin,
    /**
     Facebook https://developers.facebook.com/
     */
    RCAttributionNetworkFacebook
};

/**
 `RCPurchases` is the entry point for Purchases.framework. It should be instantiated as soon as your app has a unique user id for your user. This can be when a user logs in if you have accounts or on launch if you can generate a random user identifier.

 @warning Only one instance of RCPurchases should be instantiated at a time! Use a configure method to let the framework handle the singleton instance for you.
 */
NS_SWIFT_NAME(Purchases)
@interface RCPurchases : NSObject

/**
 Enable automatic collection of Apple Search Ads attribution. Disabled by default
 */
@property (class, nonatomic, assign) BOOL automaticAppleSearchAdsAttributionCollection;

/**
 Enable debug logging. Useful for debugging issues with the lovely team @RevenueCat
*/
@property (class, nonatomic, assign) BOOL debugLogsEnabled;

/**
 Configures an instance of the Purchases SDK with a specified API key. The instance will be set as a singleton. You should access the singleton instance using [RCPurchases sharedPurchases]

 @note Use this initializer if your app does not have an account system. `RCPurchases` will generate a unique identifier for the current device and persist it to `NSUserDefaults`. This also affects the behavior of `restoreTransactionsForAppStoreAccount`.

 @param APIKey The API Key generated for your app from https://app.revenuecat.com/

 @return An instantiated `RCPurchases` object that has been set as a singleton.
 */
+ (instancetype)configureWithAPIKey:(NSString *)APIKey;

/**
 Configures an instance of the Purchases SDK with a specified API key and app user ID. The instance will be set as a singleton. You should access the singleton instance using [RCPurchases sharedPurchases]

 @note Best practice is to use a salted hash of your unique app user ids.

 @warning Use this initializer if you have your own user identifiers that you manage.

 @param APIKey The API Key generated for your app from https://app.revenuecat.com/

 @param appUserID The unique app user id for this user. This user id will allow users to share their purchases and subscriptions across devices. Pass nil if you want `RCPurchases` to generate this for you.

 @return An instantiated `RCPurchases` object that has been set as a singleton.
 */
+ (instancetype)configureWithAPIKey:(NSString *)APIKey appUserID:(nullable NSString *)appUserID;

/**
 Configures an instance of the Purchases SDK with a custom userDefaults. Use this constructor if you want to sync status across a shared container, such as between a host app and an extension. The instance of the Purchases SDK will be set as a singleton. You should access the singleton instance using [RCPurchases sharedPurchases]

 @param APIKey The API Key generated for your app from https://app.revenuecat.com/

 @param appUserID The unique app user id for this user. This user id will allow users to share their purchases and subscriptions across devices. Pass nil if you want `RCPurchases` to generate this for you.

 @param observerMode Set this to TRUE if you have your own IAP implementation and want to use only RevenueCat's backend. Default is FALSE.

 @return An instantiated `RCPurchases` object that has been set as a singleton.
 */
+ (instancetype)configureWithAPIKey:(NSString *)APIKey
                          appUserID:(nullable NSString *)appUserID
                       observerMode:(BOOL)observerMode;

/**
 Configures an instance of the Purchases SDK with a custom userDefaults. Use this constructor if you want to sync status across a shared container, such as between a host app and an extension. The instance of the Purchases SDK will be set as a singleton. You should access the singleton instance using [RCPurchases sharedPurchases]
 
 @param APIKey The API Key generated for your app from https://app.revenuecat.com/
 
 @param appUserID The unique app user id for this user. This user id will allow users to share their purchases and subscriptions across devices. Pass nil if you want `RCPurchases` to generate this for you.
 
 @param observerMode Set this to TRUE if you have your own IAP implementation and want to use only RevenueCat's backend. Default is FALSE.

 @param userDefaults Custom userDefaults to use
 
 @return An instantiated `RCPurchases` object that has been set as a singleton.
 */
+ (instancetype)configureWithAPIKey:(NSString *)APIKey
                          appUserID:(nullable NSString *)appUserID
                       observerMode:(BOOL)observerMode
                       userDefaults:(nullable NSUserDefaults *)userDefaults;

/**
 Indicates whether the user is allowed to make payments.
 */
+ (BOOL)canMakePayments;

/**
 @return A singleton `RCPurchases` object. Call this after a configure method to access the singleton.
 */
@property (class, nonatomic, readonly) RCPurchases *sharedPurchases;

#pragma mark Configuration

/** Set this to true if you are passing in an appUserID but it is anonymous, this is true by default if you didn't pass an appUserID
 If a user tries to purchase a product that is active on the current app store account, we will treat it as a restore and alias
 the new ID with the previous id.
 */
@property (nonatomic) BOOL allowSharingAppStoreAccount;

/// Default to YES, set this to NO if you are finishing transactions with your own StoreKit queue listener
@property (nonatomic) BOOL finishTransactions;

/// This version of the Purchases framework
+ (NSString *)frameworkVersion;

/// Delegate for `RCPurchases` instance. The delegate is responsible for handling promotional product purchases and changes to purchaser information.
@property (nonatomic, weak, nullable) id<RCPurchasesDelegate> delegate;

#pragma mark Identity

/// The `appUserID` used by `RCPurchases`. If not passed on initialization this will be generated and cached by `RCPurchases`.
@property (nonatomic, readonly) NSString *appUserID;

/// If the `appUserID` has been generated by RevenueCat
@property (nonatomic, readonly) BOOL isAnonymous;

/**
 This function will alias two appUserIDs together.
 @param alias The new appUserID that should be linked to the currently identified appUserID
 @param completion An optional completion block called when the aliasing has been successful. This completion block will receive an error if there's been one.
 */
- (void)createAlias:(NSString *)alias completionBlock:(nullable RCReceivePurchaserInfoBlock)completion
NS_SWIFT_NAME(createAlias(_:_:));

/**
 This function will identify the current user with an appUserID. Typically this would be used after a logout to identify a new user without calling configure
 @param appUserID The appUserID that should be linked to the currently user
 @param aliasAnonymous If current user is annymous create alias with new user
 */
- (void)identify:(NSString *)appUserID aliasAnonymous: (BOOL) aliasAnonymous completionBlock:(nullable RCReceivePurchaserInfoBlock)completion
NS_SWIFT_NAME(identify(_:aliasAnonymous:_:));

/**
 * Resets the Purchases client clearing the saved appUserID. This will generate a random user id and save it in the cache.
 */
- (void)resetWithCompletionBlock:(nullable RCReceivePurchaserInfoBlock)completion
NS_SWIFT_NAME(reset(_:));

#pragma mark Attribution

/**
 Send your attribution data to RevenueCat so you can track the revenue generated by your different campaigns.

 @param data Dictionary provided by the network. See https://docs.revenuecat.com/docs/attribution
 @param network Enum for the network the data is coming from, see `RCAttributionNetwork` for supported networks
 */
+ (void)addAttributionData:(NSDictionary *)data
               fromNetwork:(RCAttributionNetwork)network;

/**
 Send your attribution data to RevenueCat so you can track the revenue generated by your different campaigns.

 @param data Dictionary provided by the network. See https://docs.revenuecat.com/docs/attribution
 @param network Enum for the network the data is coming from, see `RCAttributionNetwork` for supported networks
 @param networkUserId User Id that should be sent to the network. Default is the current App User Id
 */
+ (void)addAttributionData:(NSDictionary *)data
               fromNetwork:(RCAttributionNetwork)network
          forNetworkUserId:(nullable NSString *)networkUserId NS_SWIFT_NAME(addAttributionData(_:from:forNetworkUserId:));

#pragma mark Purchases

/**
 Get latest available purchaser info.
 
 @param completion A completion block called when purchaser info is available and not stale. Called immediately if purchaser info is cached. Purchaser info can be nil if an error occurred.
 */
- (void)purchaserInfoWithCompletionBlock:(RCReceivePurchaserInfoBlock)completion
NS_SWIFT_NAME(purchaserInfo(_:));

/**
 Fetch the configured offerings for this users. Offerings allows you to configure your in-app products vis RevenueCat and greatly simplifies management. See the guide (https://docs.revenuecat.com/entitlements) for more info.
 
 Offerings will be fetched and cached on instantiation so that, by the time they are needed, your prices are loaded for your purchase flow. Time is money.
 
 @param completion A completion block called when offerings are available. Called immediately if offerings are cached. Offerings will be nil if an error occurred.
 */
- (void)offeringsWithCompletionBlock:(RCReceiveOfferingsBlock)completion NS_SWIFT_NAME(offerings(_:));

/**
 Fetches the `SKProducts` for your IAPs for given `productIdentifiers`. Use this method if you aren't using `-offeringsWithCompletionBlock:`.
 You should use offerings though.

 @note `completion` may be called without `SKProduct`s that you are expecting. This is usually caused by iTunesConnect configuration errors. Ensure your IAPs have the "Ready to Submit" status in iTunesConnect. Also ensure that you have an active developer program subscription and you have signed the latest paid application agreements. If you're having trouble see: https://www.revenuecat.com/2018/10/11/configuring-in-app-products-is-hard

 @param productIdentifiers A set of product identifiers for in app purchases setup via iTunesConnect. This should be either hard coded in your application, from a file, or from a custom endpoint if you want to be able to deploy new IAPs without an app update.
 @param completion An @escaping callback that is called with the loaded products. If the fetch fails for any reason it will return an empty array.
 */
- (void)productsWithIdentifiers:(NSArray<NSString *> *)productIdentifiers
                     completionBlock:(RCReceiveProductsBlock)completion
NS_SWIFT_NAME(products(_:_:));

/**
 Use this function if you are not using the Offerings system to purchase an `SKProduct`. If you are using the Offerings system, use `-[RCPurchases purchasePackage:withCompletionBlock]` instead.
 
 Call this method when a user has decided to purchase a product. Only call this in direct response to user input.
 
 From here `Purchases` will handle the purchase with `StoreKit` and call the `RCPurchaseCompletedBlock`.
 
 @note You do not need to finish the transaction yourself in the completion callback, Purchases will handle this for you.
 
 @param product The `SKProduct` the user intends to purchase
 
 @param completion A completion block that is called when the purchase completes. If the purchase was successful there will be a `SKPaymentTransaction` and a `RCPurchaserInfo`. If the purchase was not successful, there will be an `NSError`. If the user cancelled, `userCancelled` will be `YES`.
 */
- (void)purchaseProduct:(SKProduct *)product withCompletionBlock:(RCPurchaseCompletedBlock)completion
NS_SWIFT_NAME(purchaseProduct(_:_:));

/**
 Purchase the passed `RCPackage`.

 Call this method when a user has decided to purchase a product. Only call this in direct response to user input.

 From here `Purchases` will handle the purchase with `StoreKit` and call the `RCPurchaseCompletedBlock`.

 @note You do not need to finish the transaction yourself in the completion callback, Purchases will handle this for you.

 @param package The `RCPackage` the user intends to purchase
 
 @param completion A completion block that is called when the purchase completes. If the purchase was successful there will be a `SKPaymentTransaction` and a `RCPurchaserInfo`. If the purchase was not successful, there will be an `NSError`. If the user cancelled, `userCancelled` will be `YES`.
 */
- (void)purchasePackage:(RCPackage *)package withCompletionBlock:(RCPurchaseCompletedBlock)completion
NS_SWIFT_NAME(purchasePackage(_:_:));

/**
 This method will post all purchases associated with the current App Store account to RevenueCat and become associated with the current `appUserID`. If the receipt is being used by an existing user, the current `appUserID` will be aliased together with the `appUserID` of the existing user. Going forward, either `appUserID` will be able to reference the same user.

 You shouldn't use this method if you have your own account system. In that case "restoration" is provided by your app passing
 the same `appUserId` used to purchase originally.

 @note This may force your users to enter the App Store password so should only be performed on request of the user. Typically with a button in settings or near your purchase UI.
 */
- (void)restoreTransactionsWithCompletionBlock:(nullable RCReceivePurchaserInfoBlock)completion
NS_SWIFT_NAME(restoreTransactions(_:));


- (void)tryToRestoreTransactionsWithCompletionBlock:(nullable RCReceivePurchaserInfoBlock)completion
NS_SWIFT_NAME(tryToRestoreTransactions(_:));

/**
 Computes whether or not a user is eligible for the introductory pricing period of a given product. You should use this method to determine whether or not you show the user the normal product price or the introductory price. This also applies to trials (trials are considered a type of introductory pricing).

 @note Subscription groups are automatically collected for determining eligibility. If RevenueCat can't definitively compute the eligibilty, most likely because of missing group information, it will return `RCIntroEligibilityStatusUnknown`. The best course of action on unknown status is to display the non-intro pricing, to not create a misleading situation. To avoid this, make sure you are testing with the latest version of iOS so that the subscription group can be collected by the SDK.

 @param productIdentifiers Array of product identifiers for which you want to compute eligibility
 @param receiveEligibility A block that receives a dictionary of product_id -> `RCIntroEligibility`.
*/
- (void)checkTrialOrIntroductoryPriceEligibility:(NSArray<NSString *> *)productIdentifiers
                                 completionBlock:(RCReceiveIntroEligibilityBlock)receiveEligibility;

/**
 Use this function to retrieve the `SKPaymentDiscount` for a given `SKProduct`.
 
 @param discount The `SKProductDiscount` to apply to the product.
 
 @param product The `SKProduct` the user intends to purchase.
 
 @param completion A completion block that is called when the `SKPaymentDiscount` is returned. If it was not successful, there will be an `NSError`.
*/
- (void)paymentDiscountForProductDiscount:(SKProductDiscount *)discount
                                  product:(SKProduct *)product
                               completion:(RCPaymentDiscountBlock)completion API_AVAILABLE(ios(12.2), macosx(10.14.4));


/**
 Use this function if you are not using the Offerings system to purchase an `SKProduct` with an applied `SKPaymentDiscount`. If you are using the Offerings system, use `-[RCPurchases purchasePackage:withDiscount:withCompletionBlock]` instead.
 
 Call this method when a user has decided to purchase a product with an applied discount. Only call this in direct response to user input.
 
 From here `Purchases` will handle the purchase with `StoreKit` and call the `RCPurchaseCompletedBlock`.
 
 @note You do not need to finish the transaction yourself in the completion callback, Purchases will handle this for you.
 
 @param product The `SKProduct` the user intends to purchase
 
 @param discount The `SKPaymentDiscount` to apply to the purchase
 
 @param completion A completion block that is called when the purchase completes. If the purchase was successful there will be a `SKPaymentTransaction` and a `RCPurchaserInfo`. If the purchase was not successful, there will be an `NSError`. If the user cancelled, `userCancelled` will be `YES`.
 */
- (void)purchaseProduct:(SKProduct *)product
           withDiscount:(SKPaymentDiscount *)discount
        completionBlock:(RCPurchaseCompletedBlock)completion NS_SWIFT_NAME(purchaseProduct(_:discount:_:)) API_AVAILABLE(ios(12.2), macosx(10.14.4));

/**
 Purchase the passed `RCPackage`.

 Call this method when a user has decided to purchase a product with an applied discount. Only call this in direct response to user input.

 From here `Purchases` will handle the purchase with `StoreKit` and call the `RCPurchaseCompletedBlock`.

 @note You do not need to finish the transaction yourself in the completion callback, Purchases will handle this for you.

 @param package The `RCPackage` the user intends to purchase

 @param discount The `SKPaymentDiscount` to apply to the purchase
 
 @param completion A completion block that is called when the purchase completes. If the purchase was successful there will be a `SKPaymentTransaction` and a `RCPurchaserInfo`. If the purchase was not successful, there will be an `NSError`. If the user cancelled, `userCancelled` will be `YES`.
 */
- (void)purchasePackage:(RCPackage *)package
           withDiscount:(SKPaymentDiscount *)discount
        completionBlock:(RCPurchaseCompletedBlock)completion NS_SWIFT_NAME(purchasePackage(_:discount:_:)) API_AVAILABLE(ios(12.2), macosx(10.14.4));

#pragma mark Unavailable Methods
#define RC_UNAVAILABLE(msg) __attribute__((unavailable(msg)));
/// :nodoc:
typedef void (^RCReceiveEntitlementsBlock)(id _Nullable, NSError * _Nullable) NS_SWIFT_NAME(Purchases.ReceiveEntitlementsBlock);
/// :nodoc:
- (void)makePurchase:(SKProduct *)product withCompletionBlock:(RCPurchaseCompletedBlock)block
NS_SWIFT_NAME(makePurchaseSwift(_:_:)) RC_UNAVAILABLE("makePurchase: has been replaced by purchaseProduct:");
/// :nodoc:
- (void)entitlementsWithCompletionBlock:(RCReceiveEntitlementsBlock)completion
NS_SWIFT_NAME(entitlements(_:)) RC_UNAVAILABLE("entitlements: has been replaced with offerings:. See https://docs.revenuecat.com/docs/offerings-migration");
/// :nodoc:
- (void)makePurchase:(SKProduct *)product
        withDiscount:(nullable SKPaymentDiscount *)discount
     completionBlock:(RCPurchaseCompletedBlock)completion NS_SWIFT_NAME(makePurchase(_:discount:_:)) API_AVAILABLE(ios(12.2), macosx(10.14.4)) __attribute__((unavailable("makePurchase:withDiscount: has been replaced by purchaseProduct:withDiscount:")));;

#undef RC_UNAVAILABLE

@end

/**
 Delegate for `RCPurchases` responsible for handling updating your app's state in response to updated purchaser info or promotional product purchases.
 
 @note Delegate methods can be called at any time after the `delegate` is set, not just in response to `purchaserInfo:` calls. Ensure your app is capable of handling these calls at anytime if `delegate` is set.
 */
NS_SWIFT_NAME(PurchasesDelegate)
@protocol RCPurchasesDelegate <NSObject>
@optional

/**
 Called whenever `RCPurchases` receives updated purchaser info. This may happen periodically
 throughout the life of the app if new information becomes available (e.g. UIApplicationDidBecomeActive).
 
 @param purchases Related `RCPurchases` object
 @param purchaserInfo Updated `RCPurchaserInfo`
 */
- (void)purchases:(RCPurchases *)purchases didReceiveUpdatedPurchaserInfo:(RCPurchaserInfo *)purchaserInfo
NS_SWIFT_NAME(purchases(_:didReceiveUpdated:));

/**
 Called when a user initiates a promotional in-app purchase from the App Store. If your app is able to handle a purchase at the current time, run the deferment block in this method. If the app is not in a state to make a purchase: cache the defermentBlock, then call the defermentBlock when the app is ready to make the promotional purchase. If the purchase should never be made, you don't need to ever call the defermentBlock and `RCPurchases` will not proceed with promotional purchases.
 
 @param product `SKProduct` the product that was selected from the app store
 */
- (void)purchases:(RCPurchases *)purchases shouldPurchasePromoProduct:(SKProduct *)product defermentBlock:(RCDeferredPromotionalPurchaseBlock)makeDeferredPurchase;

@end

NS_ASSUME_NONNULL_END
