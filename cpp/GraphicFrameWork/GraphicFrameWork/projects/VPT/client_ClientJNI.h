/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class client_ClientJNI */

#ifndef _Included_client_ClientJNI
#define _Included_client_ClientJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     client_ClientJNI
 * Method:    cppMain
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_client_ClientJNI_cppMain
  (JNIEnv *, jclass, jobjectArray);

/*
 * Class:     client_ClientJNI
 * Method:    recievePacket
 * Signature: (Lcommon/networking/packet/Packet;)V
 */
JNIEXPORT void JNICALL Java_client_ClientJNI_recievePacket
  (JNIEnv *, jclass, jobject);

/*
 * Class:     client_ClientJNI
 * Method:    socketClosed
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_client_ClientJNI_socketClosed
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
