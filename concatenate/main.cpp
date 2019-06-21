//
//  main.cpp
//  concatenate
//
//  Created by 骁 on 2019/6/19.
//  Copyright © 2019 zhou xiao. All rights reserved.
//

#include <iostream>
#include "Concatenate.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    const char usage[] = "Usage: concatenate -a audio_dir -f frame_file_path -o synthesized_audio_file_path";
    if(argc<=1)
    {
        printf("%s\n", usage);
        exit(false);
    }
    
    char audio_dir[MAX_PATH_LEN];
    char frame_file_path[MAX_PATH_LEN];
    char synthesized_audio_file_path[MAX_PATH_LEN];
    
    for(int i=1; i<argc;i++)
    {
        if(argv[i][0]=='-')
        {
            switch (argv[i][1]) {
                case 'a':
                    strcpy(audio_dir, argv[++i]);
                    break;
                case 'f':
                    strcpy(frame_file_path, argv[++i]);
                    break;
                case 'o':
                    strcpy(synthesized_audio_file_path, argv[++i]);
                    break;
                default:
                    break;
            }
        }
    }
    Concatenate(audio_dir, frame_file_path, synthesized_audio_file_path);
    return 0;
}
