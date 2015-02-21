//
//  test2.cpp
//  NEURONET
//
//  Created by Vinay Sriram on 2/21/15.
//  Copyright (c) 2015 Vinay Sriram. All rights reserved.
//

#include "main.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <stdio.h>
#include <string.h>

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"

void logEmoState(unsigned int userID, EmoStateHandle eState);

int main(int argc, char** argv) {
    
    EmoEngineEventHandle eEvent = EE_EmoEngineEventCreate();
    EmoStateHandle eState = EE_EmoStateCreate();
    unsigned int userID = 0;
    int state  = 0;
    std::string input;
    
    try {
        
        if (EE_EngineConnect() != EDK_OK) {
            throw std::runtime_error("Emotiv Engine start up failed.");
        }
        
        while (true) {
            
            state = EE_EngineGetNextEvent(eEvent);
            
            // New event needs to be handled
            if (state == EDK_OK) {
                EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
                EE_EmoEngineEventGetUserId(eEvent, &userID);
                
                // Log the EmoState if it has been updated
                if (eventType == EE_EmoStateUpdated) {
                    
                    EE_EmoEngineEventGetEmoState(eEvent, eState);
                    
                    logEmoState(userID, eState);
                }
            }
            else if (state != EDK_NO_EVENT) {
                std::cout << "Internal error in Emotiv Engine!" << std::endl;
                break;
            }
        }
        
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cout << "Press any key to exit..." << std::endl;
        getchar();
    }
    
    EE_EngineDisconnect();
    EE_EmoStateFree(eState);
    EE_EmoEngineEventFree(eEvent);
    
    return 0;
}


void logEmoState(unsigned int userID, EmoStateHandle eState) {

    
    // Log the time stamp and user ID
    std::cout << ES_GetTimeFromStart(eState) << " | ";
    std::cout << userID << " | ";
    std::cout << static_cast<int>(ES_GetWirelessSignalStatus(eState)) << " | ";
    
    
    // Affectiv Suite results
    std::cout << ES_GetTimeFromStart(eState) << " | ";
    std::cout << ES_AffectivGetExcitementShortTermScore(eState) << " | ";
    std::cout << ES_AffectivGetExcitementLongTermScore(eState) << " | ";
    std::cout << ES_AffectivGetEngagementBoredomScore(eState) << " | ";
    std::cout << ES_AffectivGetMeditationScore(eState) << " | ";
    std::cout << ES_AffectivGetFrustrationScore(eState) << " | ";
    std::cout << ES_AffectivGetValenceScore(eState) << " |" << std::endl;
}