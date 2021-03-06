/*
 * This file is part of the SDWebImage package.
 * (c) Olivier Poitrey <rs@dailymotion.com>
 * (c) Jamie Pinkham
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#import "SDWebImageCompat.h"

FOUNDATION_EXPORT NSErrorDomain const _Nonnull SDWebImageErrorDomain;

/// The HTTP status code for invalid download response (NSNumber *)
FOUNDATION_EXPORT NSErrorUserInfoKey const _Nonnull SDWebImageErrorDownloadStatusCodeKey;

/// SDWebImage error domain and codes
/// SDWebImage 错误域和代码
typedef NS_ERROR_ENUM(SDWebImageErrorDomain, SDWebImageError) {
    
    // The URL is invalid, such as nil URL or corrupted URL
    // 该 URL 无效，例如 nil URL 或已损坏的 URL
    SDWebImageErrorInvalidURL = 1000,
    SDWebImageErrorBadImageData = 1001, // The image data can not be decoded to image, or the image data is empty
    SDWebImageErrorCacheNotModified = 1002, // The remote location specify that the cached image is not modified, such as the HTTP response 304 code. It's useful for `SDWebImageRefreshCached`
    SDWebImageErrorInvalidDownloadOperation = 2000, // The image download operation is invalid, such as nil operation or unexpected error occur when operation initialized
    SDWebImageErrorInvalidDownloadStatusCode = 2001, // The image download response a invalid status code. You can check the status code in error's userInfo under `SDWebImageErrorDownloadStatusCodeKey`
    SDWebImageErrorCancelled = 2002, // The image loading operation is cancelled before finished, during either async disk cache query, or waiting before actual network request. For actual network request error, check `NSURLErrorDomain` error domain and code.
};
