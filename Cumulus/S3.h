//
//  S3.h
//  VaporApp
//
//  Created by Nick Jensen on 4/8/15.
//  Copyright (c) 2015 Nick Jensen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonHMAC.h>

static inline NSString * S3DateString() {
    
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"EEE, d MMM yyyy HH:mm:ss zzzz"];
    [dateFormatter setTimeZone:[NSTimeZone timeZoneWithName:@"GMT"]];
    [dateFormatter setLocale:[[NSLocale alloc] initWithLocaleIdentifier:@"en_US"]];
    NSString *date = [dateFormatter stringFromDate:[NSDate date]];
    return date;
}

static inline NSString * S3AuthHeader(NSString *action, NSString *contentType, NSString *date, NSString *acl, NSString *resource, NSString *accessKey, NSString *secretAccessKey) {
    
    assert([action length]);
    assert([date length]);
    assert([resource length]);
    assert([accessKey length]);
    assert([secretAccessKey length]);
    
    NSString *request = [NSString stringWithFormat:@"%@\n\n%@\n%@", action, contentType, date];
    
    if ([acl length]) {
        
        request = [request stringByAppendingFormat:@"\n%@", acl];
    }
    
    request = [request stringByAppendingFormat:@"\n%@", resource];
    
    NSData *clearTextData = [request dataUsingEncoding:NSUTF8StringEncoding];
    NSData *keyData = [secretAccessKey dataUsingEncoding:NSUTF8StringEncoding];
    
    uint8_t digest[CC_SHA1_DIGEST_LENGTH] = {0};
    
    CCHmacContext hmacContext;
    CCHmacInit(&hmacContext, kCCHmacAlgSHA1, keyData.bytes, keyData.length);
    CCHmacUpdate(&hmacContext, clearTextData.bytes, clearTextData.length);
    CCHmacFinal(&hmacContext, digest);
    
    NSData *data = [NSData dataWithBytes:digest length:CC_SHA1_DIGEST_LENGTH];
    
    static const char encodingTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    if ([data length] == 0)
        return @"";
    
    char *characters = malloc((([data length] + 2) / 3) * 4);
    
    if (characters == NULL) {
        
        return nil;
    }
    
    NSUInteger length = 0;
    NSUInteger i = 0;
    while (i < [data length]) {
        
        char buffer[3] = {0, 0, 0};
        short bufferLength = 0;
        
        while (bufferLength < 3 && i < [data length]) {
            
            buffer[bufferLength++] = ((char *)[data bytes])[i++];
        }
        
        //  Encode the bytes in the buffer to four characters, including padding "=" characters if necessary.
        characters[length++] = encodingTable[(buffer[0] & 0xFC) >> 2];
        characters[length++] = encodingTable[((buffer[0] & 0x03) << 4) | ((buffer[1] & 0xF0) >> 4)];
        
        if (bufferLength > 1) {
            
            characters[length++] = encodingTable[((buffer[1] & 0x0F) << 2) | ((buffer[2] & 0xC0) >> 6)];
        }
        else {
            
            characters[length++] = '=';
        }
        if (bufferLength > 2) {
            
            characters[length++] = encodingTable[buffer[2] & 0x3F];
        }
        else {
            
            characters[length++] = '=';
        }
    }
    
    NSString *signature = [[NSString alloc] initWithBytesNoCopy:characters length:length encoding:NSASCIIStringEncoding freeWhenDone:YES];
    NSString *header = [[NSString alloc] initWithFormat:@"AWS %@:%@", accessKey, signature];
    
    return header;
}