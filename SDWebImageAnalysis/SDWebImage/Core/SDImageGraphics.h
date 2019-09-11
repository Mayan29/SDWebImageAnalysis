/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"
#import <CoreGraphics/CoreGraphics.h>

// These following graphics context method are provided to easily write cross-platform(AppKit/UIKit) code.
// For UIKit, these methods just call the same method in `UIGraphics.h`. See the documentation for usage.
// For AppKit, these methods use `NSGraphicsContext` to create image context and match the behavior like UIKit.
// 提供以下图形上下文方法，以便轻松编写跨平台（AppKit/UIKit）代码。
// 对于 UIKit，这些方法只调用 `UIGraphics.h` 中的相同方法。有关用法，请参阅文档。
// 对于 AppKit，这些方法使用 `NSGraphicsContext` 创建图像上下文，并匹配类似 UIKit 的行为。

/// Returns the current graphics context.
/// 返回当前图形上下文。
FOUNDATION_EXPORT CGContextRef __nullable SDGraphicsGetCurrentContext(void) CF_RETURNS_NOT_RETAINED;
/// Creates a bitmap-based graphics context and makes it the current context.
/// 创建基于位图的图形上下文并使其成为当前上下文。
FOUNDATION_EXPORT void SDGraphicsBeginImageContext(CGSize size);
/// Creates a bitmap-based graphics context with the specified options.
/// 使用指定的选项创建基于位图的图形上下文。
FOUNDATION_EXPORT void SDGraphicsBeginImageContextWithOptions(CGSize size, BOOL opaque, CGFloat scale);
/// Removes the current bitmap-based graphics context from the top of the stack.
/// 从堆栈顶部移除当前基于位图的图形上下文。
FOUNDATION_EXPORT void SDGraphicsEndImageContext(void);
/// Returns an image based on the contents of the current bitmap-based graphics context.
/// 根据当前基于位图的图形上下文的内容返回图像。
FOUNDATION_EXPORT UIImage * __nullable SDGraphicsGetImageFromCurrentImageContext(void);
