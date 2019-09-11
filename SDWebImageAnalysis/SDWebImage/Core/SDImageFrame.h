/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>
#import "SDWebImageCompat.h"

// This class is used for creating animated images via `animatedImageWithFrames` in `SDImageCoderHelper`.
// @note If you need to specify animated images loop count, use `sd_imageLoopCount` property in `UIImage+Metadata.h`.
// 这个 class 用于通过 `SDImageCoderHelper` 中的 `animatedImageWithFrames` 创建动画图像。
// 注意：如果需要指定动画图像循环计数，请在 `UIImage+Metadata.h` 中使用 `sd_imageLoopCount` 属性。
@interface SDImageFrame : NSObject

/**
 The image of current frame. You should not set an animated image.
 */
@property (nonatomic, strong, readonly, nonnull) UIImage *image;
/**
 The duration of current frame to be displayed. The number is seconds but not milliseconds. You should not set this to zero.
 */
@property (nonatomic, readonly, assign) NSTimeInterval duration;

/**
 Create a frame instance with specify image and duration

 @param image current frame's image
 @param duration current frame's duration
 @return frame instance
 */
+ (instancetype _Nonnull)frameWithImage:(UIImage * _Nonnull)image duration:(NSTimeInterval)duration;

@end
