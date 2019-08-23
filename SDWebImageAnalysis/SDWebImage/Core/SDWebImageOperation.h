/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import <Foundation/Foundation.h>

/// A protocol represents cancelable operation.
/// 表示可取消的操作
@protocol SDWebImageOperation <NSObject>

- (void)cancel;

@end

/// NSOperation conform to `SDWebImageOperation`.
@interface NSOperation (SDWebImageOperation) <SDWebImageOperation>

@end
