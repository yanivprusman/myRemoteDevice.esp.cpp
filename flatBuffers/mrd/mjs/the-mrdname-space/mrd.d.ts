import * as flatbuffers from 'flatbuffers';
import { actions } from '../the-mrdname-space/actions.js';
import { types } from '../the-mrdname-space/types.js';
export declare class MRD {
    bb: flatbuffers.ByteBuffer | null;
    bb_pos: number;
    __init(i: number, bb: flatbuffers.ByteBuffer): MRD;
    static getRootAsMRD(bb: flatbuffers.ByteBuffer, obj?: MRD): MRD;
    static getSizePrefixedRootAsMRD(bb: flatbuffers.ByteBuffer, obj?: MRD): MRD;
    type(): types;
    action(): actions;
    data(): string | null;
    data(optionalEncoding: flatbuffers.Encoding): string | Uint8Array | null;
    static startMRD(builder: flatbuffers.Builder): void;
    static addType(builder: flatbuffers.Builder, type: types): void;
    static addAction(builder: flatbuffers.Builder, action: actions): void;
    static addData(builder: flatbuffers.Builder, dataOffset: flatbuffers.Offset): void;
    static endMRD(builder: flatbuffers.Builder): flatbuffers.Offset;
    static finishMRDBuffer(builder: flatbuffers.Builder, offset: flatbuffers.Offset): void;
    static finishSizePrefixedMRDBuffer(builder: flatbuffers.Builder, offset: flatbuffers.Offset): void;
    static createMRD(builder: flatbuffers.Builder, type: types, action: actions, dataOffset: flatbuffers.Offset): flatbuffers.Offset;
}
