<script src="../flatBuffers/mrd/flatbuffers.min.js"></script>
<script src="../flatBuffers/mrd/js/mrd.min.js"></script>
<script>
function doIt() {
    let builder = new flatbuffers.Builder(1024);
    let dataOffset = builder.createString("Hello, MRD!");
    mrd.theMRDNameSpace.MRD.startMRD(builder);
    mrd.theMRDNameSpace.MRD.addType(builder, mrd.theMRDNameSpace.types.server);
    mrd.theMRDNameSpace.MRD.addAction(builder, mrd.theMRDNameSpace.actions.getPage); 
    mrd.theMRDNameSpace.MRD.addData(builder, dataOffset); 
    let mrdOffset = mrd.theMRDNameSpace.MRD.endMRD(builder);
    builder.finish(mrdOffset);
    let buf = builder.asUint8Array();
    console.log("Serialized MRD:", buf);
    //
    let buf2 = new flatbuffers.ByteBuffer(buf); 
    let amrd = mrd.theMRDNameSpace.MRD.getRootAsMRD(buf2);
    console.log("MRD Type:", amrd.type()); 
    console.log("MRD Action:", amrd.action()); 
    console.log("MRD Data:", amrd.data()); 
}
doIt();
</script>