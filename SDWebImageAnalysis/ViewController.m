//
//  ViewController.m
//  SDWebImageAnalysis
//
//  Created by mayan on 2019/8/22.
//  Copyright © 2019 mayan. All rights reserved.
//

#import "ViewController.h"
#import "UIImageView+WebCache.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UIImageView *imgView = [[UIImageView alloc] initWithFrame:CGRectMake((self.view.bounds.size.width - 272) / 2, 200, 272, 92)];
    [self.view addSubview:imgView];
    
    [imgView sd_setImageWithURL:[NSURL URLWithString:@"https://www.google.com.hk/images/branding/googlelogo/1x/googlelogo_color_272x92dp.png"]
               placeholderImage:[UIImage imageNamed:@"placeholderImage"]
                        options:0
                        context:nil
                       progress:^(NSInteger receivedSize, NSInteger expectedSize, NSURL * _Nullable targetURL) {
                           NSLog(@"接收大小: %ld, 预期大小: %ld, url: %@", receivedSize, expectedSize, targetURL);
                       }
                      completed:^(UIImage * _Nullable image, NSError * _Nullable error, SDImageCacheType cacheType, NSURL * _Nullable imageURL) {
                          NSLog(@"image: %@, error: %@, 图像存储类型: %ld, url: %@", image, error, (long)cacheType, imageURL);
                      }];
}


@end
