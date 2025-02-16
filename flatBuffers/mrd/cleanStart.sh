find . ! -name 'cleanStart.sh' ! -name 'mrd.fbs' ! -name 'mrd_generated.h' ! -name 'tsconfig.json' ! -name 'flatbuffers.min.js' ! -name 'package.json' ! -path "./node_modules" ! -path "./node_modules/**" ! -name '.' -exec rm -rf {} +

find . -name "*.ts" -exec sh -c '
    echo "//File: {}"
    # Remove single-line comments, then remove multi-line comments
    cat {} | \
    sed '"'"'s|//.*$||g'"'"' | \
    sed '"'"':a;N;$!ba;s|/\*.*\*/||g'"'"' | \
    # Remove empty lines that might result from comment removal
    sed '"'"'/^[[:space:]]*$/d'"'"'
' \; > out.txt

