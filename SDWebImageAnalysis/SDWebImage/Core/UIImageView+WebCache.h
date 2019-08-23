/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import "SDWebImageManager.h"

// Integrates SDWebImage async downloading and caching of remote images with UIImageView.
// 使用 UIImageView 集成 SDWebImage 异步下载和远程图像缓存
@interface UIImageView (WebCache)

- (void)sd_setImageWithURL:(nullable NSURL *)url NS_REFINED_FOR_SWIFT;

- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder NS_REFINED_FOR_SWIFT;

- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options NS_REFINED_FOR_SWIFT;

// context: A context contains different options to perform specify changes or processes, see `SDWebImageContextOption`. This hold the extra objects which `options` enum can not hold.
// context: 上下文包含执行指定更改或进程的不同选项，请参见 `SDWebImageContextOption`。这将保存 `options` 枚举无法保存的额外对象。
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                   context:(nullable SDWebImageContext *)context;

// completedBlock: A block called when operation has been completed. This block has no return value and takes the requested UIImage as first parameter. In case of error the image parameter is nil and the second parameter may contain an NSError. The third parameter is a Boolean indicating if the image was retrieved from the local cache or from the network.The fourth parameter is the original image url.
// completedBlock: 操作完成后调用的 block。这个 block 没有返回值，将请求的 UIImage 作为第一个参数。如果出现错误，图像参数为 nil，第二个参数可能包含 NSError。第三个参数是一个布尔值，表示是从本地缓存还是从网络中检索图像。第四个参数是原始图像 URL。
- (void)sd_setImageWithURL:(nullable NSURL *)url
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                 completed:(nullable SDExternalCompletionBlock)completedBlock NS_REFINED_FOR_SWIFT;

- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

// progressBlock: A block called while image is downloading
// @note the progress block is executed on a background queue
// progressBlock：下载图像时调用的 block
// 注意：进度 block 是在后台队列上执行的。
- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

- (void)sd_setImageWithURL:(nullable NSURL *)url
          placeholderImage:(nullable UIImage *)placeholder
                   options:(SDWebImageOptions)options
                   context:(nullable SDWebImageContext *)context
                  progress:(nullable SDImageLoaderProgressBlock)progressBlock
                 completed:(nullable SDExternalCompletionBlock)completedBlock;

@end
