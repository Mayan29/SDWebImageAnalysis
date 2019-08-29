/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

/// Operation execution order
/// 操作执行顺序
typedef NS_ENUM(NSInteger, SDWebImageDownloaderExecutionOrder) {
    
    // Default value. All download operations will execute in queue style (first-in-first-out).
    // 默认值。所有下载操作将以队列形式执行（先进先出）。
    SDWebImageDownloaderFIFOExecutionOrder,
    
    // All download operations will execute in stack style (last-in-first-out).
    // 所有下载操作将以堆栈形式执行（后进先出）。
    SDWebImageDownloaderLIFOExecutionOrder
};

// The class contains all the config for image downloader
// @note This class conform to NSCopying, make sure to add the property in `copyWithZone:` as well.
// 此类包含图像下载器的所有配置
// 注意：此类符合 NSCopying，请确保在 `copyWithZone:` 中添加属性
@interface SDWebImageDownloaderConfig : NSObject <NSCopying>

// Gets the default downloader config used for shared instance or initialization when it does not provide any downloader config. Such as `SDWebImageDownloader.sharedDownloader`.
// @note You can modify the property on default downloader config, which can be used for later created downloader instance. The already created downloader instance does not get affected.
// 获取当共享实例或初始化不提供任何下载程序配置时使用的默认下载程序配置。例如 `SDWebImageDownloader.sharedDownloader`
// 注意：您可以修改默认下载程序配置的属性，该属性可用于以后创建的下载程序实例。已创建的下载程序实例不会受到影响。
@property (nonatomic, class, readonly, nonnull) SDWebImageDownloaderConfig *defaultDownloaderConfig;

// The maximum number of concurrent downloads.
// Defaults to 6.
// 并发下载的最大数量
// 默认为 6
@property (nonatomic, assign) NSInteger maxConcurrentDownloads;

// The timeout value (in seconds) for each download operation.
// Defaults to 15.0.
// 每次下载操作的超时 value（以秒为单位）
// 默认为 15.0
@property (nonatomic, assign) NSTimeInterval downloadTimeout;

// The minimum interval about progress percent during network downloading. Which means the next progress callback and current progress callback's progress percent difference should be larger or equal to this value. However, the final finish download progress callback does not get effected.
// The value should be 0.0-1.0.
// @note If you're using progressive decoding feature, this will also effect the image refresh rate.
// @note This value may enhance the performance if you don't want progress callback too frequently.
// Defaults to 0, which means each time we receive the new data from URLSession, we callback the progressBlock immediately.
// 网络下载过程中进度百分比的最小间隔。这意味着下一个进度回调和当前进度回调的进度百分比差异应该大于或等于此值。但是，最终完成下载进度回调不会生效。
// value 应为 0.0 - 1.0
// 注意：如果您使用渐进式解码功能，这也会影响图像刷新率。
// 注意：如果您不希望过于频繁地进行回调，则此值可能会提高性能。
// 默认为 0，这意味着每次从 URLSession 接收新数据时，我们立即回调 progressBlock。
@property (nonatomic, assign) double minimumProgressInterval;

// The custom session configuration in use by NSURLSession. If you don't provide one, we will use `defaultSessionConfiguration` instead.
// Defatuls to nil.
// @note This property does not support dynamic changes, means it's immutable after the downloader instance initialized.
// NSURLSession 正在使用的自定义会话配置。如果您不提供，我们将使用 `defaultSessionConfiguration`。
// 默认为 nil
// 注意：此属性不支持动态更改，意味着它在 downloader 实例初始化后是不可变的。
@property (nonatomic, strong, nullable) NSURLSessionConfiguration *sessionConfiguration;

// Gets/Sets a subclass of `SDWebImageDownloaderOperation` as the default `NSOperation` to be used each time SDWebImage constructs a request operation to download an image.
// Defaults to nil.
// @note Passing `NSOperation<SDWebImageDownloaderOperation>` to set as default. Passing `nil` will revert to `SDWebImageDownloaderOperation`.
// 获取或者设置 `SDWebImageDownloaderOperation` 的子类作为默认的 `NSOperation`，在 SDWebImage 每次构造下载图像的请求操作时使用该子类。
// 默认为 nil
// 注意：将 `NSOperation<SDWebImageDownloaderOperation>` 设置为默认值。设置 `nil` 将恢复为 `SDWebImageDownloaderOperation`。
@property (nonatomic, assign, nullable) Class operationClass;

// Changes download operations execution order.
// Defaults to `SDWebImageDownloaderFIFOExecutionOrder`.
// 更改下载操作执行顺序。
// 默认为 `SDWebImageDownloaderFIFOExecutionOrder`
@property (nonatomic, assign) SDWebImageDownloaderExecutionOrder executionOrder;

// Set the default URL credential to be set for request operations.
// Defaults to nil.
// 设置要为请求操作设置的默认 URL 证书。
// 默认为 nil。
@property (nonatomic, copy, nullable) NSURLCredential *urlCredential;

// Set username using for HTTP Basic authentication.
// Defaults to nil.
// 为 HTTP 基本身份验证设置用户名。
// 默认为 nil。
@property (nonatomic, copy, nullable) NSString *username;

// Set password using for HTTP Basic authentication.
// Defautls to nil.
// 使用 HTTP 基本身份验证设置密码。
// 默认为 nil。
@property (nonatomic, copy, nullable) NSString *password;

@end
