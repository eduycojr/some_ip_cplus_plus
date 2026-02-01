$buildDir = "..\..\build\Release"

Write-Host "=== Running Unit Tests ===" -ForegroundColor Cyan

Write-Host "`n[TEST] Serialization Test:" -ForegroundColor Yellow
& "$buildDir\test_serialization.exe"

Write-Host "`n[TEST] End-to-End Test:" -ForegroundColor Yellow
& "$buildDir\test_endtoend.exe"

Write-Host "`n=== Running Client-Server Demo ===" -ForegroundColor Cyan

# Start server in background
Write-Host "`n[DEMO] Starting server..." -ForegroundColor Yellow
$server = Start-Process -FilePath "$buildDir\server_example.exe" -PassThru -WindowStyle Normal

Start-Sleep -Seconds 2

# Run client
Write-Host "[DEMO] Running client..." -ForegroundColor Yellow
& "$buildDir\client_example.exe"

# Stop server (safe)
Write-Host "`n[DEMO] Stopping server..." -ForegroundColor Yellow

if ($null -ne $server) {
    # Try to get a current process object for the started process id
    $proc = Get-Process -Id $server.Id -ErrorAction SilentlyContinue

    if ($null -ne $proc) {
        try {
            # Try graceful close first (works for GUI apps / processes with windows)
            if ($proc.CloseMainWindow()) {
                Write-Host "Sent close request to process $($server.Id). Waiting up to 5s for exit..." -ForegroundColor Yellow
                $proc.WaitForExit(5000)
            }

            # If it's still running, escalate to Kill
            if (-not $proc.HasExited) {
                Write-Host "Process $($server.Id) did not exit; killing..." -ForegroundColor Yellow
                $proc.Kill()
                $proc.WaitForExit(5000)
            }

            if ($proc.HasExited) {
                Write-Host "Server process $($server.Id) stopped." -ForegroundColor Green
            } else {
                Write-Warning "Server process $($server.Id) did not exit after kill attempt."
                # Final attempt with Stop-Process (force, but silently continue on error)
                Stop-Process -Id $server.Id -Force -ErrorAction SilentlyContinue
            }
        } catch {
            Write-Warning "Error stopping process $($server.Id): $_. Attempting Stop-Process as fallback."
            Stop-Process -Id $server.Id -Force -ErrorAction SilentlyContinue
        }
    } else {
        Write-Host "Process $($server.Id) not found   it may have already exited." -ForegroundColor Yellow
    }
} else {
    Write-Warning "No server process object available to stop."
}
Write-Host "`n=== All Tests Complete ===" -ForegroundColor Green