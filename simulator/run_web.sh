#!/bin/bash
# Quick start script for web-based ILI9225 simulator (GitHub Codespaces compatible)

echo "========================================"
echo "ILI9225 LCD Web Simulator - Quick Start"
echo "========================================"
echo ""

# Check if dependencies are installed
if ! python3 -c "import flask" 2>/dev/null; then
    echo "Installing dependencies..."
    pip install -q -r requirements-web.txt
fi

PORT=${1:-5000}

echo "Starting web simulator on port $PORT..."
echo ""
echo "🌐 The simulator will open in your browser automatically"
echo "📡 WebSocket and REST API endpoints are available"
echo ""
echo "Keyboard shortcuts in browser:"
echo "  - Click buttons to test different features"
echo "  - 🗑️ Clear - Clear the display"
echo "  - ▶️ Run Demo - Automated demonstration"
echo "  - 🔴 Test Pixels - Pixel drawing test"
echo "  - ⬜ Test Shapes - Shape drawing test"
echo "  - ⭕ Test Circles - Circle drawing test"
echo "  - 📝 Test Text - Text rendering test"
echo ""

python3 web_simulator.py --demo --port $PORT
