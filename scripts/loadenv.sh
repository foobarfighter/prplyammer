#!/bin/bash

if [ -e "../.env" ]; then
  source ../.env
else
  echo "Could not find .env file"
fi  

echo ""
echo "Running with environment variables:"
echo "==================================="
echo "CLIENT_ID:     $CLIENT_ID"
echo "CLIENT_SECRET: $CLIENT_SECRET"
echo "TEST_TOKEN:    $TEST_TOKEN"
echo ""
