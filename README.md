# Borsh for Unreal 5.x

`BorshUnreal` is an implementation of the borsh serialization specification for Unreal Engine version 5.x. It is based on [borsh-cpp20](https://github.com/etodanik/borsh-cpp20).

## Motivation

This plugin is written as part of a suite of modules to enable Solana blockchain support for Unreal Engine, but can be used anywhere Borsh serialization / deserialization is required. 

## Current state

The following types are addded on top of the `std` types covered by the upstream library:

- [x] USTRUCT (including nested struct support)
- [x] FString
- [x] TArray of any supported type
- [ ] ... more will be added

## Caveats
Support for Unreal types is added in a slightly lazy fashion right now, by converting them to `std` types as an interim stage.
