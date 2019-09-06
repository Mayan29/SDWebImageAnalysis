/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageDefine.h"
#import "SDWebImageOperation.h"

typedef void(^SDImageLoaderProgressBlock)(NSInteger receivedSize, NSInteger expectedSize, NSURL * _Nullable targetURL);
typedef void(^SDImageLoaderCompletedBlock)(UIImage * _Nullable image, NSData * _Nullable data, NSError * _Nullable error, BOOL finished);

#pragma mark - Context Options

// A `UIImage` instance from `SDWebImageManager` when you specify `SDWebImageRefreshCached` and image cache hit.
// This can be a hint for image loader to load the image from network and refresh the image from remote location if needed. If the image from remote location does not change, you should call the completion with `SDWebImageErrorCacheNotModified` error. (UIImage)
// @note If you don't implement `SDWebImageRefreshCached` support, you do not need to care abot this context option.
// `SDWebImageManager` 中的 `UIImage` 实例，当您指定 `SDWebImageRefreshCached` 和图像缓存时。
// 这是一个提示，image loader 从网络加载图像并在需要时从远程位置刷新图像。如果远程位置的图像没有更改，则应在完成中调用 `SDWebImageErrorCacheNotModified` 错误。（图像）
// 注意：如果不实现 `SDWebImageRefreshCached` 支持，则不需要关心此上下文选项。
FOUNDATION_EXPORT SDWebImageContextOption _Nonnull const SDWebImageContextLoaderCachedImage;

#pragma mark - Helper method

// This is the built-in decoding process for image download from network or local file.
// @note If you want to implement your custom loader with `requestImageWithURL:options:context:progress:completed:` API, but also want to keep compatible with SDWebImage's behavior, you'd better use this to produce image.
// 这是用于从网络或本地文件下载图像的内置解码过程。
// 注意：如果要使用 `requestImageWithURL:options:context:progress:completed:` API 实现自定义加载器，但还希望兼容 SDWebImage，则最好使用此方法生成图像。
FOUNDATION_EXPORT UIImage * _Nullable SDImageLoaderDecodeImageData(NSData * _Nonnull imageData, NSURL * _Nonnull imageURL, SDWebImageOptions options, SDWebImageContext * _Nullable context);

// This is the built-in decoding process for image progressive download from network. It's used when `SDWebImageProgressiveLoad` option is set. (It's not required when your loader does not support progressive image loading)
// @note If you want to implement your custom loader with `requestImageWithURL:options:context:progress:completed:` API, but also want to keep compatible with SDWebImage's behavior, you'd better use this to produce image.
// operation: The loader operation associated with current progressive download. Why to provide this is because progressive decoding need to store the partial decoded context for each operation to avoid conflict. You should provide the operation from `loadImageWithURL:` method return value.
// 这是用于从网络渐进式下载图像的内置解码过程。当设置 `SDWebImageProgressiveLoad` 选项时使用。（加载程序不支持渐进式图像加载时不需要）
// 注意：如果要使用 `requestImageWithURL:options:context:progress:completed:` API 实现自定义加载器，但还希望兼容 SDWebImage，则最好使用此方法生成图像。
// operation: 与当前渐进式下载相关联的加载器操作。为什么提供这个是因为渐进式解码需要为每个操作存储部分解码的上下文以避免冲突。你应该从 `loadImageWithURL:` 方法返回值中提供操作。
FOUNDATION_EXPORT UIImage * _Nullable SDImageLoaderDecodeProgressiveImageData(NSData * _Nonnull imageData, NSURL * _Nonnull imageURL, BOOL finished,  id<SDWebImageOperation> _Nonnull operation, SDWebImageOptions options, SDWebImageContext * _Nullable context);

#pragma mark - SDImageLoader

// This is the protocol to specify custom image load process. You can create your own class to conform this protocol and use as a image loader to load image from network or any avaiable remote resources defined by yourself.
// If you want to implement custom loader for image download from network or local file, you just need to concentrate on image data download only. After the download finish, call `SDImageLoaderDecodeImageData` or `SDImageLoaderDecodeProgressiveImageData` to use the built-in decoding process and produce image (Remember to call in the global queue). And finally callback the completion block.
// If you directlly get the image instance using some third-party SDKs, such as image directlly from Photos framework. You can process the image data and image instance by yourself without that built-in decoding process. And finally callback the completion block.
// @note It's your responsibility to load the image in the desired global queue(to avoid block main queue). We do not dispatch these method call in a global queue but just from the call queue (For `SDWebImageManager`, it typically call from the main queue).
// 这是指定自定义图像加载过程的协议。你可以创建自己的类以符合此协议，并将其用作图像加载器，从网络或自己定义的任何可用远程资源加载图像。
// 如果你想实现从网络或本地文件下载图像的自定义加载器，只需专注于下载图像数据即可。下载完成后，调用 `SDImageLoaderDecodeImageData` 或 `SDImageLoaderDecodeProgressiveImageData` 以使用内置解码过程并生成图像（请记住调用全局队列）。最后回调完成 block。
// 如果你使用某些第三方 SDK 直接获取图像实例，例如直接从 Photos framework 获取图像。你可以自己处理图像数据和图像实例，而无需使用内置解码过程。最后回调完成 block。
// 注意：你有责任将图像加载到所需的全局队列中（以避免阻塞主队列）。我们只从调用队列，而不是全局队列中调用这些方法（对于 `SDWebImageManager`，它通常从主队列调用）。
@protocol SDImageLoader <NSObject>

// Whether current image loader supports to load the provide image URL.
// This will be checked everytime a new image request come for loader. If this return NO, we will mark this image load as failed. If return YES, we will start to call `requestImageWithURL:options:context:progress:completed:`.
// return YES to continue download, NO to stop download.
// 当前 image loader 是否支持加载提供的图像 URL。
// loader 每次有新的图像请求时都会检查这个。如果返回 NO，则将此图像加载标记为失败。如果返回 YES，我们将开始调用 `requestImageWithURL:options:context:progress:completed:`
// 返回 YES 继续下载，返回 NO 停止下载。
- (BOOL)canRequestImageForURL:(nullable NSURL *)url;

// Load the image and image data with the given URL and return the image data. You're responsible for producing the image instance.
// url: The URL represent the image. Note this may not be a HTTP URL
// context: A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
// progressBlock: A block called while image is downloading. Note the progress block is executed on a background queue
// return An operation which allow the user to cancel the current request.
// 使用给定的 URL 加载图像和图像数据，并返回图像数据。你负责生成 image 实例。
// url: 图像 URL。注意这可能不是 HTTP URL
// context: 上下文包含不同的选项来执行指定的更改或进程，请参见 `SDWebImageContextOption`。这将保存 `options` 枚举无法保存的额外对象。
// progressblock: 下载图像时调用的 block。注意进度 block 是在后台队列上执行
// 返回一个允许用户取消当前请求的操作。
- (nullable id<SDWebImageOperation>)requestImageWithURL:(nullable NSURL *)url
                                                options:(SDWebImageOptions)options
                                                context:(nullable SDWebImageContext *)context
                                               progress:(nullable SDImageLoaderProgressBlock)progressBlock
                                              completed:(nullable SDImageLoaderCompletedBlock)completedBlock;


// Whether the error from image loader should be marked indded un-recoverable or not.
// If this return YES, failed URL which does not using `SDWebImageRetryFailed` will be blocked into black list. Else not.
// url: The URL represent the image. Note this may not be a HTTP URL
// return: Whether to block this url or not. Return YES to mark this URL as failed.
// 将 image loader 中的错误标记为可恢复或不可恢复。
// 如果返回 YES，则不使用 `SDWebImageRetryFailed` 的失败 URL 将被阻止进入黑名单。否则不是。
// url: 图像 URL。注意这可能不是 HTTP URL
// 返回是否阻止此 URL。返回 YES 将此 URL 标记为失败。
- (BOOL)shouldBlockFailedURLWithURL:(nonnull NSURL *)url
                              error:(nonnull NSError *)error;

@end
