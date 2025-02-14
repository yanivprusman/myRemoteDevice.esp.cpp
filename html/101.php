<script>
// Loading a WebAssembly module
WebAssembly.instantiateStreaming(fetch('example.wasm'))
  .then(obj => {
    // Call a function exported from WebAssembly
    const result = obj.instance.exports.addNumbers(5, 3);
    console.log(result); // 8
});
</script>