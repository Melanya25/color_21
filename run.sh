cat > run.sh << 'EOF'
#!/bin/bash
export LOGIN_USER=admin
export LOGIN_PASS=1234
./server
EOF
chmod +x run.sh
