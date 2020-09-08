//
//  JTProximitySDK.h
//  JTProximitySDK
//
//  Copyright © 2020 Jointag Srl. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <JTProximitySDK/JTPLogger.h>

#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000

#import <UserNotifications/UserNotifications.h>

#endif

@protocol JTProximityCustomDelegate<NSObject>

- (void)jtProximityDidReceiveCustomAction:(nullable NSString *)customAction;

@end

NS_SWIFT_NAME(ProximitySDK)
@interface JTProximitySDK : NSObject

@property (class, readonly, nonatomic, nonnull) NSString* sdkVersion;

@property (class, readonly, nonatomic) NSUInteger sdkBuild;

@property (readonly, nonatomic, nullable) NSString *apiKey;

@property (readonly, nonatomic, nullable) NSString *apiSecret;

/**
 Enable/Disable delivering of advertising. Setting this property to "NO"
 takes precedence over any other delivery-related settings (ie GDPR consent or
 server-side setting).

 Default to YES
 */
@property (assign, nonatomic) BOOL advertisingEnabled;

/**
@deprecated Notification permission is no longer prompted automatically
*/
@property (assign, nonatomic) BOOL promptForPushNotifications DEPRECATED_MSG_ATTRIBUTE("Notification permission is no longer prompted automatically");

/**
 @deprecated Location permission is no longer prompted automatically
 */
@property (assign, nonatomic) BOOL promptForLocationAuthorization DEPRECATED_MSG_ATTRIBUTE("Location permission is no longer prompted automatically");

/**
 Enable CMP support for GDPR compliance.

 @note MUST be set before calling initWithLaunchOptions:apiKey:apiSecret:
 */
@property (assign, nonatomic) BOOL cmpEnabled;

/**
 Delegate for custom advertising actions

 @see JTProximityCustomDelegate
 */
@property (weak, nonatomic, nullable) id<JTProximityCustomDelegate> customDelegate;

/**
 UISceneDelegate for iOS 13
 */

@property (weak, nonatomic, nullable) id<UIWindowSceneDelegate> windowSceneDelegate API_AVAILABLE(ios(13.0));

/**
 Singleton instance for JTProximitySDK
 */
@property (class, readonly, strong) JTProximitySDK * _Nonnull sharedInstance NS_SWIFT_NAME(shared);

+ (nonnull instancetype)_sharedInstance NS_SWIFT_NAME(instance()) __deprecated_msg("Use ProximitySDK.shared instead");

- (void)setLogLevel:(JTPLogLevel)logLevel;

/**
 Initialization method

 Must be called from UIApplicationDelegate's application:didFinishLaunchingWithOptions: method
 */
- (void)initWithLaunchOptions:(nullable NSDictionary *)launchOptions apiKey:(nonnull NSString *)apiKey apiSecret:(nonnull NSString *)apiSecret;

// Pre iOS 10
- (BOOL)application:(nonnull UIApplication *)application didReceiveLocalNotification:(nonnull UILocalNotification *)notification __IOS_DEPRECATED(10.0, 10.0, "Old notification framework callback");

// iOS 10 only
- (BOOL)userNotificationCenter:(nonnull UNUserNotificationCenter *)center willPresentNotification:(nonnull UNNotification *)notification __IOS_AVAILABLE(10.0);

// iOS 10 only
- (BOOL)userNotificationCenter:(nonnull UNUserNotificationCenter *)center didReceiveNotificationResponse:(nonnull UNNotificationResponse *)response __IOS_AVAILABLE(10.0);

/**
 Unique installation identifier
 */
- (nonnull NSString *)installationId;

/**
 Platform-specific advertising identifier
 */
- (nullable NSString *)advertisingId;

/**
 Helper method to request tracking permissions using
 Apple AppTrackingTransparency framework
 */
- (void)requestTrackingAuthorization;

/**
 Helper method to request notifications permissions
 */
- (void)requestNotificationAuthorization;

/**
 Helper method to request location permissions
 */
- (void)requestLocationAuthorization;

/**
 Set the GDPR consent manually for the current installation.
 This method is intended as a replacement for the implementation of a IAB-compatible CMP platform

 @note cmpEnabled must be set to YES before using this method
 @see cmpEnabled
 */
- (void)setGDPRConsent:(BOOL)consent forVendor:(int)vendorId;

/**
 Get current installation GDPR consent status.

 @return true if the user consent has been given manually, or through a
 IAB-compatible CMP platform if present and the user is subjected to GDPR
 regulation.
 */
- (BOOL)getGDPRConsentForVendor:(int) vendorId;

@end