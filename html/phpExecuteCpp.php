<?php
function compile_and_run($cpp_file) {
    // 'g++ %s -o %s 2>&1'
    // "g++ -o websocket_client websocket_client.cpp -lwebsockets -pthread";
    // 'g++ %s -o %s -lixwebsocket -pthread 2>&1'
    $source_path = __DIR__ . '/' . $cpp_file;
    $output_path = substr($source_path, 0, -4) . '.out';
    $compile_command = sprintf('g++ %s -o %s -lixwebsocket -lz -pthread 2>&1', escapeshellarg($source_path), escapeshellarg($output_path));
    $descriptors = [1 => ["pipe", "w"], 2 => ["pipe", "w"]];
    $process = proc_open($compile_command, $descriptors, $pipes, __DIR__, $env);
    if (is_resource($process)) {
        $compile_output = stream_get_contents($pipes[1]) . stream_get_contents($pipes[2]);
        fclose($pipes[1]);
        fclose($pipes[2]);
        proc_close($process);
    } else {
        return "Failed to start compilation process";
    }
    if (!file_exists($output_path)) {
        return "Compilation failed: " . $compile_output;
    }
    $run_command = escapeshellarg($output_path) . " 2>&1";
    $output = shell_exec($run_command);
    unlink($output_path);
    return $output;
}
$result = compile_and_run('cpp/program.cpp');
echo $result;