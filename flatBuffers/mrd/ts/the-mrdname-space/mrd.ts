// automatically generated by the FlatBuffers compiler, do not modify

/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */

import * as flatbuffers from 'flatbuffers';

import { actions } from '../the-mrdname-space/actions.js';
import { types } from '../the-mrdname-space/types.js';


export class MRD {
  bb: flatbuffers.ByteBuffer|null = null;
  bb_pos = 0;
  __init(i:number, bb:flatbuffers.ByteBuffer):MRD {
  this.bb_pos = i;
  this.bb = bb;
  return this;
}

static getRootAsMRD(bb:flatbuffers.ByteBuffer, obj?:MRD):MRD {
  return (obj || new MRD()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

static getSizePrefixedRootAsMRD(bb:flatbuffers.ByteBuffer, obj?:MRD):MRD {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new MRD()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
}

type():types {
  const offset = this.bb!.__offset(this.bb_pos, 4);
  return offset ? this.bb!.readInt8(this.bb_pos + offset) : types.server;
}

action():actions {
  const offset = this.bb!.__offset(this.bb_pos, 6);
  return offset ? this.bb!.readInt8(this.bb_pos + offset) : actions.getPage;
}

data():string|null
data(optionalEncoding:flatbuffers.Encoding):string|Uint8Array|null
data(optionalEncoding?:any):string|Uint8Array|null {
  const offset = this.bb!.__offset(this.bb_pos, 8);
  return offset ? this.bb!.__string(this.bb_pos + offset, optionalEncoding) : null;
}

static startMRD(builder:flatbuffers.Builder) {
  builder.startObject(3);
}

static addType(builder:flatbuffers.Builder, type:types) {
  builder.addFieldInt8(0, type, types.server);
}

static addAction(builder:flatbuffers.Builder, action:actions) {
  builder.addFieldInt8(1, action, actions.getPage);
}

static addData(builder:flatbuffers.Builder, dataOffset:flatbuffers.Offset) {
  builder.addFieldOffset(2, dataOffset, 0);
}

static endMRD(builder:flatbuffers.Builder):flatbuffers.Offset {
  const offset = builder.endObject();
  return offset;
}

static finishMRDBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset);
}

static finishSizePrefixedMRDBuffer(builder:flatbuffers.Builder, offset:flatbuffers.Offset) {
  builder.finish(offset, undefined, true);
}

static createMRD(builder:flatbuffers.Builder, type:types, action:actions, dataOffset:flatbuffers.Offset):flatbuffers.Offset {
  MRD.startMRD(builder);
  MRD.addType(builder, type);
  MRD.addAction(builder, action);
  MRD.addData(builder, dataOffset);
  return MRD.endMRD(builder);
}
}
