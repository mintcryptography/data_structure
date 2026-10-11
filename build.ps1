# Build and run all data-structure unit tests (Windows / MinGW g++).
#
# Usage:
#   .\build.ps1              Build and run all 5 tests
#   .\build.ps1 queue        Only run the Queue test
#
# Requires g++ on PATH (MinGW or MSYS2 both work).

param(
    [string]$Only = ""
)

$ErrorActionPreference = "Stop"

$projectRoot = $PSScriptRoot
$buildDir = Join-Path $projectRoot "build"

if (!(Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

$tests = @(
    @{ name = "linkedlist";  src = "tests/test_linkedlist.cpp" }
    @{ name = "hashmap";     src = "tests/test_hashmap.cpp"    }
    @{ name = "queue";       src = "tests/test_queue.cpp"      }
    @{ name = "graph";       src = "tests/test_graph.cpp"      }
    @{ name = "bipartite";   src = "tests/test_bipartite.cpp"  }
)

if ($Only -ne "") {
    $tests = $tests | Where-Object { $_.name -eq $Only }
    if ($tests.Count -eq 0) {
        Write-Error "Test not found: $Only (options: linkedlist / hashmap / queue / graph / bipartite)"
    }
}

$totalFail = 0
foreach ($t in $tests) {
    $exe = Join-Path $buildDir "test_$($t.name).exe"
    $src = Join-Path $projectRoot $t.src

    Write-Host ""
    Write-Host "=== Build test_$($t.name) ===" -ForegroundColor Cyan
    & g++ -std=c++11 -Wall -Wextra -O2 -o $exe $src
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Compile failed: $($t.src)" -ForegroundColor Red
        $totalFail++
        continue
    }

    Write-Host "--- Run test_$($t.name) ---" -ForegroundColor Yellow
    & $exe
    if ($LASTEXITCODE -ne 0) {
        $totalFail++
    }
}

Write-Host ""
if ($totalFail -eq 0) {
    Write-Host "All tests passed." -ForegroundColor Green
} else {
    Write-Host "$totalFail test(s) failed." -ForegroundColor Red
    exit 1
}
