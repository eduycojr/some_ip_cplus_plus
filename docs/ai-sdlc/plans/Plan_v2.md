# WBS-2.4 — Plan v2: Revised Implementation Plan with Decision Record

## Purpose
Plan v2 addresses the challenge log findings and contains explicit decisions, required tests, and reviewer decisions before implementation begins.

## Decision record summary
| Finding | Decision | Rationale |
|---|---|---|
| timeout/retry semantics | fix | correctness risk for transport path |
| lock ordering | fix | prevents deadlock under concurrency |
| tail light port isolation | fix | keep example apps isolated |
| inconsistent error semantics | fix | standardize response generation |
| rollback strategy | fix | required for transport-risk work |
| CTest registration | fix | required to allow real test execution |
| silent drop handling | fix | ensure not masked by logging gaps |
| coverage/sanitizer scope | fix | required for evidence quality |
| approval gate | fix | protect architecture and change scope |
| ctest -C config requirement | fix | prevents false green reports |

## Plan v2 — Implementation order and tasks

### Stage A — Safety and gate checks
1. Freeze the approved requirement baseline from WBS-2.2 and record it in manifest.
2. Re-run build/test under Release configuration and capture logs.
3. Confirm CTest registration for the repo and keep the tests command in the runbook.

### Stage B — ISSUE-02 timeout/retry path
1. Add timeout/retry configuration parameters to `include/someip/transport.hpp` and update `src/transport.cpp`.
2. Add 3-attempt retry loop, fixed delay, and error mapping for timeout.
3. Add regression tests for default timeout, custom timeout, retry exhaustion, and no false positive on normal traffic.
4. Validate with `ctest -C Release --output-on-failure`.

### Stage C — ISSUE-03 synchronization and error handling
1. Identify shared mutable state in `examples/server_app.cpp` and any handler state that can race.
2. Add mutex/atomic protections and explicit logging for null endpoint handling.
3. Add concurrent request tests and sanitizer-capable run instructions for Linux/WSL.
4. Validate message routing error semantics against `MessageType::ERR` and `E_TIMEOUT`.

### Stage D — ISSUE-01 tail light service example
1. Add tail light service constants, examples, and standalone server/client.
2. Validate ports, stable state, and hazard/on-off semantics.
3. Add end-to-end tests for service registration and error response generation.
4. Validate no regression of the existing brake demo or general API behavior.

## Implementation policy
- Read and review source before writing code.
- Every changed hunk must have a one-line rationale recorded in the PR description.
- No code changes may bypass the test run gate.
- Any unverified claim must be marked BLOCKED in the handover.

## Reviewer approval gate
Required before coding begins:
- human reviewer approves Plan v2
- challenge log issues are accepted as addressed or explicitly deferred with rationale
- implementation order is accepted

## Evidence required before implementation begins
- Approved WBS-2.2 artifact committed and recorded in manifest
- WBS-2.3 repo exploration evidence present
- Build/test logs configured under Release and ctest -C Release
- Plan v2 handover created and ready for execution
