# WBS-2.3 Hallucination Log (Draft)

Task: `WBS-2.3 Repository exploration on an unfamiliar codebase`
Repository: `eduycojr/some_ip_cplus_plus`
Branch: `ai-assisted-sdlc`
Baseline commit: `e4a83be7dd313322714370996a4fe3511f134cde`
Status: DRAFT

## Purpose
Record incorrect AI claims discovered during WBS-2.3 repository exploration and document the correction, likely cause, and prevention step.

| Log ID | Incorrect claim | Ground truth | Evidence source | Likely cause | Corrective / prevention action |
|---|---|---|---|---|---|
| H01 | Tail-light example files were absent from the repository. | Tail-light example files are present. | Branch tree listing: `examples/taillight_server_app.cpp`, `examples/taillight_client_app.cpp` | Stale or mixed-revision inspection during earlier exploration | Freeze one baseline commit before claim verification; verify against branch HEAD or explicitly named commit only. |
| H02 | `UdpEndpoint` had no retry/backoff logic in `send_to()`. | `send_to()` retries with backoff in current `src/transport.cpp`. | `src/transport.cpp` | Earlier inspection used an outdated file revision | Record file blob SHA with every claim and re-verify after branch changes. |
| H03 | `UdpEndpoint` did not configure socket receive timeout. | Current `src/transport.cpp` sets `SO_RCVTIMEO` to 100 ms. | `src/transport.cpp` | Earlier inspection was stale and overgeneralized | Re-read current source before concluding capability absence. |
| H04 | Only two tests existed in the repository. | Current branch includes at least five C++ test sources plus a PowerShell test script. | Branch tree listing under `tests/` | Earlier exploration used incomplete file discovery | Use recursive tree listing to inventory test files before summarizing test strategy. |
| H05 | Tail-light service implementation was entirely missing. | Tail-light app sources now exist in `examples/`. | Branch tree listing | Claim became stale after repository evolution | Timestamp and baseline-lock all exploration outputs; mark superseded findings when branch advances. |

## Follow-up actions
- Link each incorrect claim back to its corresponding row in `WBS-2.3_claim_verification_register.md`.
- Add any newly discovered incorrect claims before sign-off.
- Update `docs/ai-sdlc/evidence/W2_learning_loops.md` with any reusable prevention lessons.
