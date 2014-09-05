//
//  MainViewController.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/26/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define APPWARP_APP_KEY         @"1bc691b6-8c0f-4247-9"
#define APPWARP_SECRET_KEY      @"139f6094-4e04-4041-9"
#define APPWARP_HOST_ADDRESS    @"191.234.54.124"

// Response Codes
#define CLIENT_NEEDS_TO_UPDATE 1

#import <AppWarp_iOS_SDK/AppWarp_iOS_SDK.h>
#import "iAd/ADBannerView.h"

@interface MainViewController : UIViewController <ChatRequestListener, ConnectionRequestListener, LobbyRequestListener, NotifyListener, RoomRequestListener, ZoneRequestListener>
{
    // TODO
}

- (IBAction)startQuickMatch:(id)sender;

- (IBAction)startQuickOfflineMatch:(id)sender;

@end
