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
    
    int length = 73;
    // We need to send the 4 bytes of length information
    printf("%c%c%c%c", (char) (length & 0xFF),
           (char) ((length>>8) & 0xFF),
           (char) ((length>>16) & 0xFF),
           (char) ((length>>24) & 0xFF));
    // Now we can output our message
    printf("{garbage:'");
    
    EmoEngineEventHandle eEvent = EE_EmoEngineEventCreate();
    EmoStateHandle eState = EE_EmoStateCreate();
    unsigned int userID = 0;
    int state  = 0;
    std::string input;
    
    
    try {
        
        if (EE_EngineConnect() != EDK_OK) {
            throw std::runtime_error("Emotiv Engine start up failed.");
        }
        bool firstEvent = false;
        while (true) {
            state = EE_EngineGetNextEvent(eEvent);
            
            // New event needs to be handled
            if (state == EDK_OK) {
                EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
                EE_EmoEngineEventGetUserId(eEvent, &userID);
                
                // Log the EmoState if it has been updated
                if (eventType == EE_EmoStateUpdated) {
                    if(!firstEvent){
                        printf("'}");
                        firstEvent = true;
                    }
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

    
    // Affectiv Suite results
    float time = ES_GetTimeFromStart(eState);
    float excitement_st = ES_AffectivGetExcitementShortTermScore(eState);
    float excitement_lt = ES_AffectivGetExcitementLongTermScore(eState);
    float boredom = ES_AffectivGetEngagementBoredomScore(eState);
    float meditation = ES_AffectivGetMeditationScore(eState);
    float frustration = ES_AffectivGetFrustrationScore(eState);
    float valence = ES_AffectivGetValenceScore(eState);
    
    char value[10000];
    sprintf(value, "{type:'emotion', time: %f, excited_short_term: %f, excited_long_term: %f, boredom: %f, meditation: %f, frustration: %f, valence: %f}", time, excitement_st, excitement_lt, boredom, meditation, frustration, valence);
    
    
    // Collect the length of the message
    unsigned int len = (unsigned int)strlen(value);
    // We need to send the 4 bytes of length information
    printf("%c%c%c%c", (char) (len & 0xFF),
           (char) ((len>>8) & 0xFF),
           (char) ((len>>16) & 0xFF),
           (char) ((len>>24) & 0xFF));
    // Now we can output our message
    printf("%s", value);
    
}