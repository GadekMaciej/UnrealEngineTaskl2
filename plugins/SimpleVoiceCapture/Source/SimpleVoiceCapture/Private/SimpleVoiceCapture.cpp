// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleVoiceCapture.h"

#define LOCTEXT_NAMESPACE "FSimpleVoiceCaptureModule"

DEFINE_LOG_CATEGORY(LogSimpleVoiceCapture);

void FSimpleVoiceCaptureModule::StartupModule()
{
	UE_LOG(LogSimpleVoiceCapture, Verbose, TEXT("Voice Capture Module Started!"))
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSimpleVoiceCaptureModule::ShutdownModule()
{
	UE_LOG(LogSimpleVoiceCapture, Verbose, TEXT("Voice Capture Module Stopped!"))
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleVoiceCaptureModule, SimpleVoiceCapture)