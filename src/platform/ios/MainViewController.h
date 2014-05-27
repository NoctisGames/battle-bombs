//
//  MainViewController.h
//  bomberparty
//
//  Created by Stephen Gowen on 2/26/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define APPWARP_API_KEY         @"b9d7c098-5fb8-4182-8"
#define APPWARP_HOST_ADDRESS    @"191.234.48.70"

#import <AppWarp_iOS_SDK/AppWarp_iOS_SDK.h>
#import "iAd/ADBannerView.h"

@interface MainViewController : UIViewController <ChatRequestListener, ConnectionRequestListener, LobbyRequestListener, NotifyListener, RoomRequestListener, ZoneRequestListener>
{
    // TODO
}

- (IBAction)startQuickMatch:(id)sender;

- (IBAction)startQuickOfflineMatch:(id)sender;

@end
