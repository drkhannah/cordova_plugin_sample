

//#import <Cordova/Cordova.h>
#import <Cordova/CDV.h>
#import "MiSnap.h"

@interface MiSnapPlugin : CDVPlugin<MiSnapViewControllerDelegate,UIImagePickerControllerDelegate>

@property(nonatomic,retain) CDVInvokedUrlCommand* cmd;

- (void) cordovaCallMiSnap:(CDVInvokedUrlCommand *)command;

@end
