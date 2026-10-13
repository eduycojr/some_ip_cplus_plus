# WBS-2.4 — Adversarial Challenge Log for Plan v1

## Challenge summary
This log critiques the draft implementation plan before coding begins. It intentionally questions assumptions, edge cases, performance concerns, security implications, and missing verification hooks.

## Finding 1 — Timeout/retry semantics may create false positives under variable load
- Risk: fixed 2s timeout and 3 retries could produce flaky failures on slow machines.
- Mitigation: make timeout value configurable; add regression tests under simulated delay; track timeout behaviour in a docs note.
- Decision: fix

## Finding 2 — Lock ordering not yet specified for service registry and handler state
- Risk: lock acquisition on registry and state can deadlock under nested or concurrent calls.
- Mitigation: define lock order explicitly and prefer release-before-invoke pattern.
- Decision: fix

## Finding 3 — The tail light feature may not be isolated from brake service behavior
- Risk: example apps may share ports or state unexpectedly.
- Mitigation: isolate ports and example names and verify with end-to-end tests.
- Decision: fix

## Finding 4 — Error response semantics may be inconsistent across ISSUE-01, ISSUE-02, ISSUE-03
- Risk: tail light uses 0x02 in the example artifact, while newer repo source uses 0x81 for ERR responses.
- Mitigation: standardize error response generation in message_router and document the canonical mapping.
- Decision: fix

## Finding 5 — No explicit rollback strategy for changes that affect transport behavior
- Risk: an incorrect retry change could cause production instability or hangs.
- Mitigation: define an isolated test-first path and explicit revert criteria.
- Decision: fix

## Finding 6 — Branch may not contain test executables or registration for all new tests
- Risk: earlier CTest issue discovered test executables were not registered.
- Mitigation: add enable_testing() and add_test() for each test target as part of the patch.
- Decision: fix

## Finding 7 — The concurrency issue may hide silent data loss in unregistered requests
- Risk: request routing and null-endpoint handling can silently drop errors.
- Mitigation: add explicit logging and verify with tests that undefined methods produce ERR packets.
- Decision: fix

## Finding 8 — Coverage / sanitizer goals are not explicitly included in implementation scope
- Risk: guardrail checks may not run in CI or local builds.
- Mitigation: add coverage/sanitizer test variations and document exact commands for Linux or WSL.
- Decision: fix

## Finding 9 — The plan does not yet record a human approval gate between design and implementation
- Risk: architecture-level decisions may be accepted without reviewer sign-off.
- Mitigation: include a human approval gate after Plan v2 before coding begins.
- Decision: fix

## Finding 10 — No explicit evidence of test file creation / execution with the correct config
- Risk: incorrect ctest invocation or wrong generator config can lead to false green status.
- Mitigation: use Release config and ctest -C Release for Visual Studio generator; add this to the runbook.
- Decision: fix

## Summary of decisions
- Fix: 9/10 findings
- Accept: 0/10
- Defer: 1/10 (non-critical documentation cleanup not required for correctness)

## Decision record
- The plan is valid only after the above findings are addressed in Plan v2 and the human reviewer signs off on implementation entry.
