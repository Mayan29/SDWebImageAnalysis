/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

#if SD_UIKIT || SD_MAC
#import "SDImageCache.h"

#if SD_UIKIT
typedef UIViewAnimationOptions SDWebImageAnimationOptions;
#else
typedef NS_OPTIONS(NSUInteger, SDWebImageAnimationOptions) {
    SDWebImageAnimationOptionAllowsImplicitAnimation = 1 << 0, // specify `allowsImplicitAnimation` for the `NSAnimationContext`
};
#endif

typedef void (^SDWebImageTransitionPreparesBlock)(__kindof UIView * _Nonnull view, UIImage * _Nullable image, NSData * _Nullable imageData, SDImageCacheType cacheType, NSURL * _Nullable imageURL);
typedef void (^SDWebImageTransitionAnimationsBlock)(__kindof UIView * _Nonnull view, UIImage * _Nullable image);
typedef void (^SDWebImageTransitionCompletionBlock)(BOOL finished);

// This class is used to provide a transition animation after the view category load image finished. Use this on `sd_imageTransition` in UIView+WebCache.h
// for UIKit(iOS & tvOS), we use `+[UIView transitionWithView:duration:options:animations:completion]` for transition animation.
// for AppKit(macOS), we use `+[NSAnimationContext runAnimationGroup:completionHandler:]` for transition animation. You can call `+[NSAnimationContext currentContext]` to grab the context during animations block.
// @note These transition are provided for basic usage. If you need complicated animation, consider to directly use Core Animation or use `SDWebImageAvoidAutoSetImage` and implement your own after image load finished.
// 此类用于在 UIView 类别加载图像完成后提供过渡动画。在 UIView+WebCache.h 中的 `sd_imageTransition` 上使用此选项。
// 对于 UIKit(iOS & tvOS)，我们使用 `+[UIView transitionWithView:duration:options:animations:completion]` 进行转换动画。
// 对于 AppKit(macOS)，我们使用 `+[NSAnimationContext runAnimationGroup:completionHandler:]` 进行转换动画。可以调用 `+[NSAnimationContext currentContext]` 在动画 block 期间获取上下文。
// 注意，这些转换是为基本用法而提供的。如果需要复杂的动画，请考虑直接使用核心动画或 `SDWebImageAvoidAutoSetImage`，并在图像加载完成后实现自己的动画。
@interface SDWebImageTransition : NSObject

// By default, we set the image to the view at the beginning of the animtions. You can disable this and provide custom set image process
// 默认情况下，我们在动画开始时将图像设置为视图。您可以禁用此功能并提供自定义图像
@property (nonatomic, assign) BOOL avoidAutoSetImage;
/**
 The duration of the transition animation, measured in seconds. Defaults to 0.5.
 */
@property (nonatomic, assign) NSTimeInterval duration;
/**
 The timing function used for all animations within this transition animation (macOS).
 */
@property (nonatomic, strong, nullable) CAMediaTimingFunction *timingFunction API_UNAVAILABLE(ios, tvos, watchos);
/**
 A mask of options indicating how you want to perform the animations.
 */
@property (nonatomic, assign) SDWebImageAnimationOptions animationOptions;
// A block object to be executed before the animation sequence starts.
// 在动画序列开始之前要执行的 block 对象。
@property (nonatomic, copy, nullable) SDWebImageTransitionPreparesBlock prepares;
// A block object that contains the changes you want to make to the specified view.
// 包含要对指定视图进行的更改的 block 对象
@property (nonatomic, copy, nullable) SDWebImageTransitionAnimationsBlock animations;
// A block object to be executed when the animation sequence ends.
// 动画结束时要执行的 block 对象
@property (nonatomic, copy, nullable) SDWebImageTransitionCompletionBlock completion;

@end

/**
 Convenience way to create transition. Remember to specify the duration if needed.
 for UIKit, these transition just use the correspond `animationOptions`. By default we enable `UIViewAnimationOptionAllowUserInteraction` to allow user interaction during transition.
 for AppKit, these transition use Core Animation in `animations`. So your view must be layer-backed. Set `wantsLayer = YES` before you apply it.
 */
@interface SDWebImageTransition (Conveniences)

/// Fade transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *fadeTransition;
/// Flip from left transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromLeftTransition;
/// Flip from right transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromRightTransition;
/// Flip from top transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromTopTransition;
/// Flip from bottom transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *flipFromBottomTransition;
/// Curl up transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *curlUpTransition;
/// Curl down transition.
@property (nonatomic, class, nonnull, readonly) SDWebImageTransition *curlDownTransition;

@end

#endif
