# JTProximitySDK Documentation

## Table of Contents

1. [Installation](#user-content-installation)
    1. [Installation using CocoaPods](#installation-using-cocoapods)
    2. [Manual Installation](#manual-installation)
2. [Project settings](#user-content-project-settings)
    1. [Location usage description](#user-content-location-usage-description)
3. [Initialization](#user-content-initialization)
    1. [Simple Initialization](#user-content-simple-initialization)
    2. [iOS 13 Scenes](#ios-13-scenes)
    3. [Handling Notifications](#user-content-handling-notifications)
    4. [Tracking users](#user-content-tracking-users)
    5. [Disable automatic permission requests](#user-content-disable-automatic-permission-requests)
    6. [Programmatically Disable Advertising](#programmatically-disable-advertising)
    7. [GDPR](#user-content-gdpr)
    8. [Receive custom events](#user-content-receive-custom-events)

## Installation

### Installation using CocoaPods

1. Install or update `CocoaPods` to version **1.9.0 or later**, which is
   essential for proper support of the new *xcframework* format.

2. In your terminal execute `pod init` from your project directory, if the
   project is not already pod-enabled

3. Edit your project Podfile and add `pod 'JTProximitySDK'` to your main
   application target

4. In your terminal execute `pod install` from your project directory.

eg:

```ruby
target 'MyApp' do
  use_frameworks!
  pod 'JTProximitySDK'
end
```

More on [CocoaPods here](https://cocoapods.org/).

### Manual Installation

Jointag Proximity SDK is distribute as a compiled **xcframework** with binaries
for both device and simulator. To add it to your project, include the
*JTProximitySDK.xcframework* as follow:

1. Drag and drop the JTProximitySDK.xcframework framework folder into your Xcode
project (remember to check *"Copy items if needed"*).

2. Make sure the JTProximitySDK.xcframework appears in the **Frameworks,
Libraries, and Embedded Content** section of the **General** tab of your project
and that the *embed* mode is **Embed & Sign**

## Project settings
You have to put in the `Info.plist` of your project the following settings:

### Location usage description

```xml
<key>NSLocationAlwaysUsageDescription</key>
<string>Location usage description</string>
<key>NSLocationAlwaysAndWhenInUseUsageDescription</key>
<string>Location usage description</string>
<key>NSLocationWhenInUseUsageDescription</key>
<string>Location usage description</string>
```

## Initialization

Place the following code inside the `UIApplicationDelegate` of your application:

### Simple Initialization

##### Objective-C

```objc
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [JTProximitySDK.sharedInstance initWithLaunchOptions:launchOptions apiKey:@"YOUR_API_KEY" apiSecret:@"YOUR_API_SECRET"];
    // Other application logics
}
```

##### Swift

```swift
func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
    ProximitySDK.shared.initWithLaunchOptions(launchOptions, apiKey: "YOUR_API_KEY", apiSecret: "YOUR_API_SECRET")
    // Other application logics
    return true
}
```

### iOS 13 Scenes

If your application make use of iOS 13 scenes, an additional requirement is to
update the SDK windowSceneDelegate reference with the current active
UIWindowSceneDelegate.

To do this you must add the following code to your current UIWindowSceneDelegate
`scene:willConnectToSession:options:` :

```swift
import UIKit
import JTProximitySDK

class SceneDelegate: UIResponder, UIWindowSceneDelegate {

    var window: UIWindow?

    func scene(_ scene: UIScene, willConnectTo session: UISceneSession, options connectionOptions: UIScene.ConnectionOptions) {
        ProximitySDK.shared.windowSceneDelegate = self
        guard let _ = (scene as? UIWindowScene) else { return }
    }
}
```

### Handling Notifications

To enable the SDK to correctly send and manager advertising notifications, you
must implement the following method in your `UIApplicationDelegate`:

##### Objective-C

```objc
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification {
    if ([[JTProximitySDK sharedInstance] application:application didReceiveLocalNotification:notification]) {
        return;
    }
    // Other application logics
}
```

##### Swift

```swift
func application(_ application: UIApplication, didReceive notification: UILocalNotification) {
    if (ProximitySDK.shared.application(application, didReceive: notification)) {
        return
    }
    // Other application logics
}
```

If you plan to support **iOS 10.0** or later, you must also add this code in
your `UNUserNotificationCenterDelegate` methods:

##### Objective-C

```objc
- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler __IOS_AVAILABLE(10.0) {
    if ([[JTProximitySDK sharedInstance] userNotificationCenter:center willPresentNotification:notification]) {
        completionHandler(UNNotificationPresentationOptionAlert|UNNotificationPresentationOptionBadge|UNNotificationPresentationOptionSound);
        return;
    }
    // Other application logics
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void (^)(void))completionHandler __IOS_AVAILABLE(10.0) {
    if ([[JTProximitySDK sharedInstance] userNotificationCenter:center didReceiveNotificationResponse:response]) {
        completionHandler();
        return;
    }
    // Other application logics
}
```

##### Swift

```swift
@available(iOS 10.0, *)
func userNotificationCenter(_ center: UNUserNotificationCenter, willPresent notification: UNNotification, withCompletionHandler completionHandler: @escaping (UNNotificationPresentationOptions) -> Void) {
    if ProximitySDK.shared.userNotificationCenter(center, willPresent: notification) {
        completionHandler([.alert, .badge, .sound])
        return
    }
    // Other application logics
}

@available(iOS 10.0, *)
func userNotificationCenter(_ center: UNUserNotificationCenter, didReceive response: UNNotificationResponse, withCompletionHandler completionHandler: @escaping () -> Void) {
    if ProximitySDK.shared.userNotificationCenter(center, didReceive: response) {
        completionHandler()
        return
    }
    // Other application logics
}
```

### Tracking users

The SDK associates each tracked event with the *IDFA*. If the *IDFA* is not
available due to a user permission denial, the device can be identified by the
*installationId*. The *installationId* identifies in particular a specific
installation of the SDK in a certain app on a certain device. If the app
containing the SDK is uninstalled and then installed again the *installationId*
will be a different one. You can retrieve the *installationId* after the
initialization of the SDK anywhere in your code with the following line:

##### Objective-C

```objc
[JTProximitySDK sharedInstance].installationId;
```

##### Swift

```swift
ProximitySDK.shared.installationId()
```

### Disable automatic permission requests

You can disable the SDK automatic location and notification permission requests
during initialization by setting to NO the following properties on
`JTProximitySDK.sharedInstance`:

- `promptForPushNotifications` : set to `NO` to disable the automatic request
  for user notifications permission
- `promptForLocationAuthorization` : set to `NO` to disable the automatic
  request for user location permission

Note: the properties must be set before calling any
`initWithLaunchOptions:apiKey:apiSecret:` method.

### Programmatically Disable Advertising

It is possible to programmatically disable/enable the advertising delivery by
setting the SDK's `advertisingEnabled` property to `false`. It is useful for
example to disable the delivery of advertising for specific users of the
application. In that case, simply change the property as soon as the user sign
in or out of the application.
The default value for the property is `true`.

##### Objective-C

```objc
// disable advertising delivery
[[JTProximitySDK sharedInstance] setAdvertisingEnabled:NO];
// enable advertising delivery
[[JTProximitySDK sharedInstance] setAdvertisingEnabled:YES];
```

##### Swift

```swift
// disable advertising delivery
ProximitySDK.shared.advertisingEnabled = false
// enable advertising delivery
ProximitySDK.shared.advertisingEnabled = true
```

### GDPR

As a publisher, you should integrate a Consent Management Platform (CMP) and
request for vendor and purpose consents as outlined in IAB Europe’s Mobile
In-App CMP API v2.0: Transparency & Consent Framework.

To ensure that the SDK support the handling of user-consent preferences when a
IAB-compatible CMP library is present, you must enable the feature through the
`ProximitySDK.setCmpEnabled:` method, which is `false` by default.

**This method must be called before the library
`initWithLaunchOptions:apiKey:apiSecret:` method to guarantee an error-free
process**.

### Receive custom events

You can receive custom advertising events (if configured in the backend) to
integrate application-specific features by using the `customDelegate` property
of `ProximitySDK` instance.

When the application user interacts with a custom-action notification, the
`jtProximityDidReceiveCustomAction:` method is invoked by passing a
`customAction` NSString object.
