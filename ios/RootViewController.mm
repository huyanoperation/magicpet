//
//  magicpetAppController.h
//  magicpet
//
//  Created by jia huyan on 11-11-21.
//  Copyright 道锋互动（北京）科技有限责任公司 2011年. All rights reserved.
//

#import "RootViewController.h"


@implementation RootViewController

#pragma mark - MobiSageAdViewDelegate 委托
#pragma mark

- (UIViewController *)viewControllerToPresent {
    
    
    return self;
}


 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        [[MobiSageManager getInstance] setPublisherID:MobiSage_PublisherId_Test];
    }
    return self;
}


-(void)InitMobiSageAdBanner
{
    // Custom initialization
    // Custom initialization
    //一般Banner广告，320X50的banner广告，设置广告轮显效果
     adBanner = [[MobiSageAdBanner alloc] initWithAdSize:Ad_320X50 withDelegate:self];
    
    //设置广告轮显方式
    [adBanner setSwitchAnimeType:Random];
    
    [self.view addSubview:adBanner];
    [adBanner release];
}

-(void)RemoveMobiSageAdBanner
{
    if( adBanner != nil )
    {
        [adBanner removeFromSuperview];
        adBanner = nil;
    }
}
/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
 
*/
// Override to allow orientations other than the default landscape orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return UIInterfaceOrientationIsLandscape( interfaceOrientation );
    
    // switch to this line if you want to set portrait view
    // return UIInterfaceOrientationIsPortrait( interfaceOrientation );
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidUnload {
    //[super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

 -(NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
     return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
        return YES;
}


- (void)dealloc {
    [super dealloc];
}


@end
