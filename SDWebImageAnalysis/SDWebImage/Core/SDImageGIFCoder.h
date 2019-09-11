/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDImageCoder.h"

// Built in coder using ImageIO that supports animated GIF encoding/decoding
// @note `SDImageIOCoder` supports GIF but only as static (will use the 1st frame).
// @note Use `SDImageGIFCoder` for fully animated GIFs. For `UIImageView`, it will produce animated `UIImage`(`NSImage` on macOS) for rendering. For `SDAnimatedImageView`, it will use `SDAnimatedImage` for rendering.
// @note The recommended approach for animated GIFs is using `SDAnimatedImage` with `SDAnimatedImageView`. It's more performant than `UIImageView` for GIF displaying(especially on memory usage)
// 内置 coder，使用 ImageIO 支持动画 GIF 编码/解码
// 注意：`SDImageIOCoder` 支持 GIF，但只支持静态（将使用第 1 帧）。
// 注意：使用 `SDImageGIFCoder` 完全动画的 GIFs。对于 `UIImageView`，它将生成动画的 `UIImage`（macOS 上的 `NSImage`）进行渲染。对于 `SDAnimatedImageView`，它将使用 `SDAnimatedImage` 进行渲染。
// 注意：动画 GIFs 的推荐方法是使用带有 `SDAnimatedImageView` 的 `SDAnimatedImage`。对于 GIF 显示，它比 `UIImageView` 性能更好（特别是在内存使用方面）
@interface SDImageGIFCoder : NSObject <SDProgressiveImageCoder, SDAnimatedImageCoder>

@property (nonatomic, class, readonly, nonnull) SDImageGIFCoder *sharedCoder;

@end
