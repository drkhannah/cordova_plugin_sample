
#import "MiSnapPlugin.h"

@implementation MiSnapPlugin

- (void) cordovaCallMiSnap:(CDVInvokedUrlCommand *)command
{
#if(__i386__ ||__x86_64__)
    //Nothing to do here, we are on simulator
#else
    self.cmd=command;
    
    //MiSnap Invocation with default parameters for check front
    NSDictionary *videoParameters = [MiSnapViewController defaultParametersForCheckFront];
    MiSnapViewController *controller = [[MiSnapViewController alloc] init];
    controller.delegate = self;
    controller.navigationController.navigationBar.hidden=YES;
    [controller setupMiSnapWithParams:videoParameters];
    
    
    NSLog(@"COMMAND:%@",command.callbackId);
    
    [self.viewController presentViewController:controller animated:NO completion:^{
        
    }];
#endif
}

#pragma mark -
#pragma mark MiSnap Delegate methods

//MiSnap Image returning delegate

- (void)miSnapFinishedReturningEncodedImage:(NSString *)encodedImage originalImage:(UIImage *)image andResults:(NSDictionary *)results {
    
    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsString:@"Captured Image"];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:self.cmd.callbackId];
    
    /*Sent the image to the web layer from native layer
     ....
     ....
     */
    
}

//MiSnap Cancel delegate

- (void)miSnapCancelledWithResults:(NSDictionary *)results {
    
    CDVPluginResult *pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_NO_RESULT messageAsString:@"Cancelled"];
    
    [self.commandDelegate sendPluginResult:pluginResult callbackId:self.cmd.callbackId];
}



@end


