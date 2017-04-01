//
//  ViewController.m
//  MyNewApp
//
//  Created by lyleKP on 2017/3/14.
//  Copyright © 2017年 lyle. All rights reserved.
//

#import "ViewController.h"
#import <MTTToolKit/MTTToolKit.h>
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    UIView * myView = [[UIView alloc]initWithFrame:CGRectMake(100, 100, 100, 100)];
    myView.backgroundColor = [UIColor redColor];
    [self.view addSubview:myView];
    [UIView animateWithDuration:0.5 animations:^{
        myView.top = 200;
    }];
    NSLog(@"%@", NSStringFromCGSize(myView.size));
    [myView myTestLog];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
