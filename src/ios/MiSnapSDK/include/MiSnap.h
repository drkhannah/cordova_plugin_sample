//
//  MiSnap.h
//  MiSnap
//
//  Created by Greg Fisch on 6/25/14.
//  Copyright (c) 2014 mitek. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UiKit/UIKit.h>


/*! @header
 
 The MiSnap SDK API is an interface that allows the app developer targeting Mitek
 Mobile Imaging servers to construct mobile device apps.
 
 The API consists of
 
 - a method for invoking the MiSnapViewController, which starts the capture session
 and captures the image
 
 - calls into the API to query and establish parameters MiSnap uses during operation
 
 - constant values that can be used by the app developer to override MiSnap defaults
 
 - callback protocols to which the app must conform in order to retrieve MiSnap results
 
 @author Mitek Engineering on 2014-03-25
 @copyright 2012-14 Mitek Systems, Inc. All rights reserved.
 @unsorted
 */

/*!
 
 Apps making use of the MiSnap SDK must conform to this protocol in order to be called by
 MiSnap for successuful capture with image data or canceled session, both with results.
 
 */
@protocol MiSnapViewControllerDelegate <NSObject>

/*! @abstract This delegate callback method is called upon successful image capture or decode.
 
 @param encodedImage for resultCode @link kMiSnapResultSuccessVideo @/link or
 @link kMiSnapResultSuccessStillCamera @/link, this will be a scaled,
 compressed and base64 encoded image, with BI/UXP data embedded as an
 Exif comment in the encoded JPEG, suitable for processing on a Mitek
 Systems mobile imaging server; for resultCode
 @link kMiSnapResultSuccessPDF417 @/link, this value will be nil.
 @param originalImage for resultCode @link kMiSnapResultSuccessVideo @/link or
 @link kMiSnapResultSuccessStillCamera @/link, this will be the original
 unmodified image; for resultCode kMiSnapResultSuccessPDF417, this value
 will be nil.
 @param results dictionary containing the result code (via key @link kMiSnapResultCode @/link).
 For resultCode @link kMiSnapResultSuccessPDF417 @/link, this dictionary
 will also contain the decoded PDF417 data (via key
 @link kMiSnapPDF417Data @/link). For image-based result codes, the value
 obtained via key @link kMiSnapMIBIData @/link will contain recorded
 environmental factors at the time the image was captured (focus quality,
 brightness level, etc.) plus user experience data during the
 auto-capture process.
 
 @param encodedImage for resultCode @link kMiSnapResultSuccessVideo @/link or
 @link kMiSnapResultSuccessStillCamera @/link, this will be a scaled,
 compressed and base64 encoded image, with BI/UXP data embedded as an
 Exif comment in the encoded JPEG, suitable for processing on a Mitek
 Systems mobile imaging server; for resultCode
 @link kMiSnapResultSuccessPDF417 @/link, this value will be nil.
 */
- (void)miSnapFinishedReturningEncodedImage:(NSString *)encodedImage
							  originalImage:(UIImage *)originalImage
								 andResults:(NSDictionary *)results;

/*!
 
 @abstract invoked if the user cancels a capture MiSnap transaction or other conditions occur
 that cause a MiSnap transaction to end without capturing an image.
 
 @discussion
 
 The result code will be @link kMiSnapResultCancelled @/link if the user touched the X Cancel
 button during capture.
 
 The result code will be @link kMiSnapResultVideoCaptureFailed @/link if the user encounters
 the @link kMiSnapMaxTimeouts @/link threshold and the setting
 @link kMiSnapAutoCaptureFailoverToStillCapture @/link was pre-set to 0 .
 
 The result code will be @link kMiSnapResultCameraNotSufficient @/link if the device does not
 support capturing a 2 Megapixel image.
 
 The results will also contain a value for the key @link kMiSnapMIBIData @/link if such data
 was captured prior to cancellation or other termination conditions.
 
 @param results dictionary containing the result code (via key @link kMiSnapResultCode @/link)
 and other information about the termination of the MiSnap transaction.
 
 */
- (void)miSnapCancelledWithResults:(NSDictionary *)results;

@end


@interface MiSnapViewController : UIViewController <AVCaptureVideoDataOutputSampleBufferDelegate>

/*! @abstract Checks whether the app has been given permission to use the camera.  Returns TRUE if it has,
    return FALSE permission was denied or pending.  If false, the app should not call MiSnap until subsequent
    calls return TRUE.  Only checks if the current iOS version >= 8.0, return TRUE if < 8.0
 */
- (bool)checkCameraPermission;

/*! @abstract a pointer back to the method implementing the callback methods MiSnap will invoke
 upon transaction termination
 */
@property (weak, nonatomic) IBOutlet NSObject<MiSnapViewControllerDelegate>* delegate;

@property (nonatomic, assign, getter=isFullScreen) BOOL fullScreen;

/*!
 
 This method accepts parameters from the calling app.
 
 Mitek's Best Practice recommendation is to have the app retrieve Active Integration
 parameters from the Mitek Imaging Server (e.g. Mobile Deposit), then adjust only those
 parameters that Mitek has recommended or which otherwise absolutely must be adjusted, and
 pass them to this method.
 
 The dictionary of parameters sent to this method will replace any parameters previously
 established via this call.
 
 If this method is not called, a set of defaults will be internally established.
 
 @abstract method to establish parameters MiSnap will use during operation
 @param params key-value pairs whose range of eligible keys are drawn from NSString* values
 in @link //apple_ref/doc/header/MiSnap.h @/link
 in group MiSnap Input Parameters keys
 
 */
- (void) setupMiSnapWithParams:(NSDictionary *)params;

//- (NSDictionary *)analyzeExternalImage:(UIImage *)image;

/*! @abstract the current MiSnap SDK version
 @return string representing the current MiSnap SDK version
 */
+ (NSString *)miSnapVersion;

/*!
 @abstract Used to identify the current set of returned MiBI values
 @return a string the represents the MiBI dataset version
 */
+ (NSString *)mibiVersion;

/*!
 supplies the default set of MiSnap SDK parameters that the current version of MiSnap will
 use in the absence of the parameters being supplied to the
 @link setupMiSnapWithParams: @/link method.
 
 @abstract all of the relevant default MiSnap parameters, useful for establishing overrides
 @return a dictionary whose keys are drawn from @link //apple_ref/doc/header/MiSnap.h @/link
 group MiSnap Input Parameters keys, and whose values consist of string
 representations of the values of each key (the domains of which are described in the
 Programmer's Guide).
 */
+ (NSDictionary*)defaultParameters;
+ (NSMutableDictionary*)defaultParametersForACH;
+ (NSMutableDictionary*)defaultParametersForCheckFront;
+ (NSMutableDictionary*)defaultParametersForCheckBack;
+ (NSMutableDictionary*)defaultParametersForRemittance;
+ (NSMutableDictionary*)defaultParametersForBalanceTransfer;
+ (NSMutableDictionary*)defaultParametersForW2;
+ (NSMutableDictionary*)defaultParametersForDriversLicense;
+ (NSMutableDictionary*)defaultParametersForLandscapeDocument;

/*!
 This method only returns the values that MiSnap uses as defaults overridden with those
 values in the supplied dictionary.  It will not establish the values as defaults.
 
 Use @link setupMiSnapWithParams: @/link is still required after invoking this method to
 override necessary MiSnap parameter defaults.
 
 This method will not insert values from the overrideValues into the returned dictionary for
 which there are not already relevant keys.
 
 It will not insert deprecated values.
 
 @abstract a convenience class method returning @link defaultParameters @/link overridden
 with the values supplied in the overrideValues param
 @param overrideValues a dictionary containing key-value pairs that will override values
 normally returned by @link defaultParameters @/link .
 
 */
+ (NSDictionary*)defaultParametersWithOverrides:(NSDictionary*)overrideValues;
@end


// Parameter names for NSDictionary representation
// Viewfinder and Document sizes

/*! @group MiSnap Input Parameters key constants
 
 @abstract Parameter names for use in NSDictionary passed to @link setupMiSnapWithParams: @/link
 */


/*!	By establishing this MiSnap parameter with a non-zero value, MiSnap will attempt to use the
 corresponding mode.
 
 By establishing this MiSnap parameter with the value 2, MiSnap will attempt to determine the
 best video frame size for the given device.
 
 This parameter replaces both the kMiSnapAllowVideoFramesMode and the kMiSnapAllowVideoFrames
 parameters.
 
 It should not be changed without consulting with Mitek Professional Services.</i>
 
 @note Values
 "0" == off<br>
 "1" == manual capture<br>
 "2" == default to the highest video resolution supported by the device<br>
 "3" == force use of 720p (i.e. 1280x720) for video frame processing (if available)<br>
 "4" == force use of 1080p (i.e. 1920x1080) for video frame processing (if available)<br>
 "5" == force use of a special hi-res capture mode for iPhone 6 and 6+ for video frame processing
 at the highest resolution supported by the device, or a special manual photo mode for older devices<br>
*/
extern NSString* const kMiSnapCaptureMode;


/*! By default, MiSnap will automatically failover to still-camera manual-capture mode after
 allowing the user to re-attempt kMiSnapMaxTimeouts times.
 
 By establishing this MiSnap parameter with the value 0, MiSnap will instead call
 @link miSnapCancelledWithResults: @/link with the results parameter containing an
 NSDictionary element with key @link kMiSnapResultCode @/link set to
 @link kMiSnapResultVideoCaptureFailed @/link after allowing the user to
 re-attempt auto-capture @link kMiSnapMaxTimeouts @/link times.
 
 @note Values
 0 == disallow automatic failover to still-camera manual capture mode<br>
 1 == allow automatic failover to still-camera manual-capture mode<br>
 default = 1
 */
extern NSString* const kMiSnapAutoCaptureFailoverToStillCapture;


/*! This parameter specifies the horizontal width as a percentage of the viewfinder
 that the document must fill before the image will be captured.  The width of the
 Guide Image will scale based on this value
 
 Default value is 800 (80%) for all document types except Driver's License where the default is 700
 
 @note Values
 range 500-1000 measured in tenths of a percent (i.e. value of 800 == 80%)<br>
 default = 800
 */
extern NSString* const kMiSnapViewfinderMinHorizontalFill;


/*!
 The number of unnecessary touches by the user that will trigger the help screen.
 
 @note Values
 Range: 2-9<br>
 Default: 4
 */
extern NSString* const kMiSnapUnnecessaryScreenTouchLimit;


/*!	The length of time (in seconds) at the very beginning of a MiSnap user session that the
 user will be allowed to initially attempt to capture an image.
 
 At the end of this time, if the default value (or any non-zero value) is in effect for
 @link kMiSnapMaxTimeouts @/link, then the video-mode tutorial/help screen will be
 automatically displayed.
 
 If the value of @link kMiSnapMaxTimeouts @/link is set to 0 and the value of
 @link kMiSnapAutoCaptureFailoverToStillCapture @/link is set to the default value of "1",
 then the transition tutorial screen will be automatically displayed.
 
 If the values of both @link kMiSnapMaxTimeouts @/link and
 @link kMiSnapAutoCaptureFailoverToStillCapture @/link are set to 0, then at the end of this
 time, the MiSnap session will end and the @link miSnapCancelledWithResults: @/link delegate
 callback will be invoked with a resultCode of @link kMiSnapResultVideoCaptureFailed @/link.
 
 When @link kMiSnapMaxTimeouts @/link is non-zero subsequent timeouts prior to the dispaly of
 either a tutorial or cancellation will be governed by the @link kMiSnapTimeout @/link,
 assuming it is equal to or larger than this value.
 
 @note Values
 range 15-90<br>
 default = 30 seconds
 */
extern NSString* const kMiSnapInitialTimeout;


/*!	The length of time (in seconds) a user is given to attempt to capture an image (with the
 exception of the initial attempt, which is goverend by @link kMiSnapInitialTimeout @/link ).
 
 This value will only have an effect if @link kMiSnapMaxTimeouts @/link is non-zero.
 
 At the end of this time, if the number of attempts by the user has not exceeded the value of
 @link kMiSnapMaxTimeouts @/link , then the video-mode tutorial/help screen will be
 automatically displayed.
 
 If the number of attempts by the user has exceeded the value of
 @link kMiSnapMaxTimeouts @/link and the value of
 @link kMiSnapAutoCaptureFailoverToStillCapture @/link is set to the default value of "1",
 then the transition tutorial screen will be automatically displayed.
 
 If the number of attempts by the user has exceeded the value of
 @link kMiSnapMaxTimeouts @/link and the value of
 @link kMiSnapAutoCaptureFailoverToStillCapture @/link is set to 0, then at the end of this
 time, the MiSnap session will end and the @link miSnapCancelledWithResults: @/link delegate
 callback will be invoked with a resultCode of @link kMiSnapResultVideoCaptureFailed @/link.
 
 The range is not allowed to be below the value of @link kMiSnapInitialTimeout @/link ; any
 value less than that parameter value will be replaced by the value of that parameteter.
 
 @note Values
 range @link kMiSnapInitialTimeout @/link-90<br>
 default = 20 seconds for all document types except Driver’s License where the default is 30
 */
extern NSString* const kMiSnapTimeout;


/*!	number of times MiSnap will automatically show the Help dialog after the timeout
 interval. (in other words, MiSnap will timeout @link kMiSnapMaxTimeouts @/link+1 times
 before failover to either still-camera manual-capture mode or invoking
 @link MiSnapViewControllerDelegate @/link callback @link miSnapCancelledWithResults: @/link
 based on the value of parameter  @link kMiSnapAutoCaptureFailoverToStillCapture @/link.
 
 @note Values
 range 0-10<br>
 default = 0 (no retries)
 */
extern NSString* const kMiSnapMaxTimeouts;


/*!	The length if time (in milliseconds) that the Tutorial is displayed upon first
 run of MiSnap by a new user.
 
 @note Values
 range 3000-10000<br>
 default = 7500 (7.5 seconds)
 */
extern NSString* const kMiSnapTutorialBrandNewUserDuration;

/*! Flag indicating whether the tutorial shows a continue button, or terminates after
 the duration expires
 @note TRUE shows button  FALSE terminates after delay
 */
extern NSString* const kMiSnapTutorialAcknowledgeEnabled;

// Image quality parameters

/*!	indicates how much to compress the image. Typically, images are compressed to reduce
 bandwidth overhead and time when transferring the image to Mitek mobile imaging servers.
 
The recommended default value has been carefully tuned for image quality purposes - Do not change default value without consulting Mitek.
 
 @note Values
 range: 0-100<br>
 0 == "minimum quality"/"maximum compression"<br>
 100 == "maximum quality"/"no compression"<br>
 default = 50
 */
extern NSString* const kMiSnapImageQuality;

// Environmental criteria to satisfy

/*!	The acceptable brightness to allow automatic image capture
 (applies to video camera only)
 
 @note Values
 range: 0 - 1000 (1000 indicates "ideal brightness"; 0 indicates ignore setting)<br>
 default = 400
 */
extern NSString* const kMiSnapBrightness;
extern NSString* const kMiSnapMinBrightness;


/*!	The maximum acceptable brightness to allow automatic image capture
 (applies to video camera only)
 
 The recommended default value has been carefully tuned for image quality purposes - Do not change default value without consulting Mitek.
 
 @note Values
 range: 0 - 1000 (1000 indicates "ideal brightness"; 0 indicates ignore setting)<br>
 default = 700
 */
extern NSString* const kMiSnapMaxBrightness;


/*!	The acceptable sharpness (where 1000 is “ideal”) to allow automatic image capture
 (applies to video camera only).
 
 The recommended default value has been carefully tuned for image quality purposes - Do not change default value without consulting Mitek.
 
 @note Values
 range: 0 - 1000 (1000 indicates "ideal brightness"; 0 indicates ignore setting)<br>
 default = Document specific: 600 for ACH/CheckFront, 100 for CheckBack, 200 for CheckBack withEndorsement Detection,
 750 for AutoInsurance, 850 for BalanceTransfer/Remittance, 400 for BusinessCard,
 350 for DriverLicense, 400 for W2, and 400 for all other types of documents.
 */
extern NSString* const kMiSnapSharpness;


/*! The minimum number of detected pixels between the document rectangle and the image frame.
 
 @note Values
 range: 0 - 1000<br>
 default = 20
 */
extern NSString* const kMiSnapAngle;

/*! The detected angle rotated from the display measured at the time of automatic image capture.

 @note Values
range 0-1000 measured in tenths of a percent (i.e. 150 == 15%)<br>
0 indicates that the angle setting is to be ignored by MiSnap during processing<br>
default = 150
*/
extern NSString* const kMiSnapAngle;

// Flash/Torch

/*!	The initial setting for the torch in MiSnap Video Auto-Capture mode.<br>
 
 For value AUTO (or AUTO+DL), MiSnap uses the @link kMiSnapBrightness @/link calculated for
 each video frame passed into MiSnap to automatically determine when to turn on the device torch
 for the user.  (If the user subsequently turns off the torch, or turns on the torch prior to
 the auto-torch calculation kicking in and then turns off the torch, the Auto-Torch calculation
 will be suspended for the remainder of that MiSnap capture session.)
 
 For @link kMiSnapDocumentType @/link containing "DRIVER_LICENSE", MiSnap will ignore the
 AUTO setting and leave the torch off (but will make it available for the user).  AUTO+DL
 ignores this MiSnap internal override.
 
 The recommended default value has been carefully tuned for image quality purposes - Do not change default value without consulting Mitek.
 
 @note Values
 2 == ON - torch on<br>
 1 == Auto - MiSnap determines if torch is needed during Video Frame processing<br>
 0 == OFF - no torch<br>
 default = 1, except if kMiSnapDocumentType contains "DRIVER_LICENSE" the recommended use is to set this to 0 
 */
extern NSString* const kMiSnapTorchMode;        // New name

// Document type

/*!	The type of document to be captured.
 
 If value @link kMiSnapDocumentTypePDF417 @/link is specified, then MiSnap will internally
 capture an image of the 2D Barcode and return the decoded string using key
 @link kMiSnapPDF417Data @/link in the @link kMiSnapResultCode @/link field in the delegate
 callback @link miSnapFinishedReturningEncodedImage:originalImage:andResults: @/link
 
 @note Values
 default = "ACH"
 • “ACH” – for capturing front of check for ACH use case
 • “CheckFront” - for capturing front of check for mobile deposit
 • “CheckBack” - for capturing back of check for mobile deposit
 • “REMITTANCE” or “BALANCE_TRANSFER” - for capturing a bill payment stub for bill pay or balance transfer use cases
 • “DRIVER_LICENSE” - for capturing the front of a Driver's License
*/
extern NSString* const kMiSnapDocumentType;


/*! A human readable description of the document type referenced in
 @link kMiSnapDocumentType @/link
 
 @note Values
 default = "ACH Enrollment"
 */
extern NSString* const kMiSnapShortDescription;


// Customizable UI Elements

/*!	This value represents whether or not to animate the final set of icons displayed as
 part of the transition from a successful capture of a document to the scene to be
 displayed after MiSnap returns to the view controller that presented it.
 
 @note Values
 0 means display the PNG file bug_animation_01.png for 2 seconds<br>
 1 means animate the MiSnap final bug<br>
 default = 1
 */
extern NSString* const kMiSnapAnimatedBug;


/*! Stroke-width(thickness) in pixels of the line representing the rectangle that is
 animated when MiSnap has determined that the user has successfully captured an image of
 a document
 
 @note Values
 range 1-100<br>
 default = 20
 */
extern NSString* const kMiSnapAnimationRectangleStrokeWidth;


/*!	Corner-radius in pixels of the line representing the rectangle that is animated
 when MiSnap has determined that the user has successfully captured an image of a document
 
 @note Values
 range 1-100<br>
 default = 16
 */
extern NSString* const kMiSnapAnimationRectangleCornerRadius;


/*!	6-character color value (HTML-like RGB format, with two hex characters per R, G, and B)
 of the rectangle that is animated when MiSnap has determined that the user has
 successfully captured an image of a document.
 
 @note Values
 range 6-character string with hex values, no prefix<br>
 default = ED1C24
 */
extern NSString* const kMiSnapAnimationRectangleColor;


/*!	Indicates if Smart Bubble feature is enabled or not
 
 If the value is YES/TRUE/1 then the Smart Bubble feature is enabled.
 Otherwise it is not enabled.
 
 @note Values
 0/NO/FALSE means Smart Bubble feature is NOT enabled<br>
 1/YES/TRUE means Smart Bubble feature IS enabled<br>
 default = 1/YES/TRUE
 */
extern NSString* const kMiSnapSmartBubbleEnabled;


/*!	When @link kMiSnapSmartBubbleEnabled @/link is YES/ON/1 then this setting
 defines how often the Smart Bubble is shown after MiSnap starts.
 
 @note Values
 range 1000-10000 milliseconds (1-10 seconds)<br>
 default = 3000 (3 seconds)
 */
extern NSString* const kMiSnapSmartBubbleAppearanceDelay;


// Guide Image feature

/*!	Indicates if Guide Image feature is enabled or not
 
 If the value is YES/TRUE/1 then the Guide Image feature is enabled.
 otherwise it is not enabled.
 
 @note Values
 0/NO/FALSE means Guide Image feature is NOT enabled<br>
 1/YES/TRUE means Guide Image feature IS enabled<br>
 default = 1/YES/TRUE
 */
extern NSString* const kMiSnapCameraGuideImageEnabled;


/*!	This value represents whether or not the ghost image fades out when the gauge bar is shown.
 
 @note Values
 0 means the ghost image will fade in and out<br>
 1 means the ghost image will always be visible<br>
 default = 0
 */
extern NSString* const kMiSnapGhostImageAlwaysOn;


/*!	Indicates if Guide Image feature is enabled or not FOR THE STILL CAMERA
 
 If the value is YES/TRUE/1 then the Guide Image feature is enabled
 when MiSnap is in Still Camera mode. Otherwise it is not enabled.
 @note Values
 0/NO/FALSE means Guide Image feature is NOT enabled<br>
 1/YES/TRUE means Guide Image feature IS enabled<br>
 default = 1/YES/TRUE
 */
extern NSString* const kMiSnapCameraGuideImageStillCameraEnabled;


/*!	Show a vignette overlay on the misnap view in Video Auto-capture mode.
 
 In the view overlay presented by MiSnapViewController, an overlay image will be used to help
 guide the user to slightly center the document and avoid the edges of the viewfinder.
 
 @note Values
 0/NO/FALSE means the Vignette feature is NOT enabled<br>
 1/YES/TRUE means the Vignette feature is enabled<br>
 default = 1/YES/TRUE
 */
extern NSString* const kMiSnapCameraVignetteImageEnabled;


/*!	Show a vignette overlay on the misnap view in Still-Camera Manual mode.
 
 In the view overlay presented by MiSnapViewController, an overlay image will be used to help
 guide the user to slightly center the document and avoid the edges of the viewfinder.
 
 @note Values
 0/NO/FALSE means the Vignette feature is NOT enabled<br>
 1/YES/TRUE means the Vignette feature is enabled<br>
 default = 1/YES/TRUE
 */
extern NSString* const kMiSnapCameraVignetteImageManualModeEnabled;

/*! @group MiSnap Input Parameters Document Type constant values */


/*! indicates MiSnap use internal parameters to attempt to capture the back of a check,
 displaying "Back of check" at the top of the capture preview display.
 */
extern NSString* const kMiSnapDocumentTypeCheckBack;


/*! indicates MiSnap use internal parameters to attempt to capture the front of a check,
 displaying "Front of check" at the top of the capture preview display.
 */
extern NSString* const kMiSnapDocumentTypeCheckFront;


/*! indicates MiSnap use internal parameters to attempt to capture a generic landscape document */
extern NSString* const kMiSnapDocumentTypeLandscapeDoc;


/*! indicates MiSnap should internally invoke the barcode reader library and return the
 text of the captured PDF417
 */
extern NSString* const kMiSnapDocumentTypePDF417;

/*! @group MiSnap Input Parameters key constants
 @abstract keys for values in NSDictionary passed to MiSnap as parameters to
 @link setupMiSnapWithParams: @/link
 */


/*! indicates MiSnap should scan for a credit card and return the
 *	text of the captured account number
 */
extern NSString* const kMiSnapDocumentTypeCreditCard;


// Server Versions that MiSnap knows about
// Versions
 extern NSString* const kMiSnapVersion;
 extern NSString* const kMiSnapServerVersion;
 extern NSString* const kMiSnapServerType;
 extern NSString* const kMiSnapMibiVersion;


/*!	@group MiSnap Output Parameters key constants
 @abstract keys for values in NSDictionary passed back as parameter to
 @link miSnapFinishedReturningEncodedImage:originalImage:andResults: @/link or
 @link miSnapCancelledWithResults: @/link
 */

/*! The key constant to access the value indicating success, cancellation, or other MiSnap
 termination conditions as passed in the @link MiSnapViewControllerDelegate @/link protocol
 method callbacks @link miSnapFinishedReturningEncodedImage:originalImage:andResults: @/link
 and @link miSnapCancelledWithResults: @/link
 
 @note Values
 @link kMiSnapResultSuccessVideo @/link , @link kMiSnapResultSuccessStillCamera @/link ,
 @link kMiSnapResultSuccessPDF417 @/link , @link kMiSnapResultCameraNotSufficient @/link ,
 @link kMiSnapResultCancelled @/link , @link kMiSnapResultVideoCaptureFailed @/link .
 */
extern NSString* const kMiSnapResultCode;


/*! The key constant to access the value containing the PDF417 data captured by the barcode
 reader library in MiSnap as passed in the @link MiSnapViewControllerDelegate @/link protocol
 method callback @link miSnapFinishedReturningEncodedImage:originalImage:andResults: @/link
 */
extern NSString* const kMiSnapPDF417Data;


/*! The key constant to access the value containing the card number string captured by the
 *  credit card reader library in MiSnap as passed in the @link MiSnapViewControllerDelegate
 *  @/link protocol method callback
 *  @link miSnapFinishedReturningEncodedImage:originalImage:andResults: @/link
 */
extern NSString* const kMiSnapCreditCardNumberString;


/*! The key constant to access the value containing the MIBI/UXP data collected during the video
 auto-capture process in MiSnap as passed in the @link MiSnapViewControllerDelegate @/link
 protocol method callbacks
 @link miSnapFinishedReturningEncodedImage:originalImage:andResults: @/link and
 @link miSnapCancelledWithResults: @/link.
 
 Mitek Best Practices Guide recommend passing this data to the Mitek imaging server to which
 the app is connected (usually via app proxy server) in the case of cancellation to ensure
 that MIBI/UXP data for abandoned sessions is collected.
 */
extern NSString* const kMiSnapMIBIData;


/*! The detected angle rotated from the display measured at the time of automatic image
 capture. 0 means there was no detected difference. This should never exceed the Angle
 input parameter.
 
 @note Values
 range: 0-1000 measured in tenths of a percent (i.e. 150 == 15%).<br>
 0 means there was no detected difference in rotation or skew.
 */
extern NSString* const kMiSnapReturnAngle;


/*!	The brightness value (where 1000 is “ideal”) measured at the time of automatic image
 capture (applies to video camera only)
 
 @note Values
 range: 0-1000 (1000 represents "ideal brightness")
 */
extern NSString* const kMiSnapReturnBrightness;


/*! The sharpness value (where 1000 is “ideal”) measured at the time of automatic image
 capture (applies to video camera only). <br><br>
 
 @note Values
 range: 0-1000 (1000 represents "ideal sharpness")
 */
extern NSString* const kMiSnapReturnSharpness;

///*! The minimum number of pixels between the document rectangle and the image frame
// @note Values
// range: 0-1000
// */
//extern NSString* const kMiSnapReturnMinPadding;

///*! A string representing the value(s) read from the MICR line if present.  Nil, or empty, if no
// MICR was found
// */
//extern NSString* const  kMiSnapReturnMICR;

/*!	An indicator of whether or not MiSnap was in still or an Auto-capture mode at the time of
 capture.<br><br>
 
 @note Values
 "0" == off<br>
 "1" == manual<br>
 "2" == default to a device appropriate setting<br>
 "3" == force use of 720p (i.e. 1280x720) for video frame processing (if available)<br>
 "4" == force use of 1080p (i.e. 1920x1080) for video frame processing (if available)<br>
 "5" == force use of a photo mode for video frame processing at the highest resolution supported by the device<br>
 */
extern NSString* const kMiSnapReturnCaptureMode;


/*!	An indicator of whether or not the torch was on or off at the end of the MiSnap session
 
 @note Values
 1 == Torch was on at end of session<br>
 0 == Torch was off at end of session
 */
extern NSString* const kMiSnapReturnLighting;

/*!	@group MiSnap Output ResultCode value constants  */


/*! MiSnap image capture transaction resulted in successful auto-capture in video-mode */
extern NSString* const kMiSnapResultSuccessVideo;


/*! MiSnap image capture transaction resulted in successful capture in still-camera mode */
extern NSString* const kMiSnapResultSuccessStillCamera;


/*! MiSnap PDF417 Capture transaction resulted in successful capture and translation */
extern NSString* const kMiSnapResultSuccessPDF417;


/*! MiSnap Credit Card Capture transaction resulted in successful capture */
extern NSString* const kMiSnapResultSuccessCreditCard;


/*! MiSnap transaction aborted due to inability of camera to perform necessary function */
extern NSString* const kMiSnapResultCameraNotSufficient;


/*! MiSnap transaction cancelled by user */
extern NSString* const kMiSnapResultCancelled;


/*! MiSnap video-mode auto-capture failed and auto-failover to still camera disabled */
extern NSString* const kMiSnapResultVideoCaptureFailed;

/*! The amount of time in milliseconds to delay before MiSnap terminates after
 a successful capture.*/
extern NSString* const kMiSnapTerminationDelay;

