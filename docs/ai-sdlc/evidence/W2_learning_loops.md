# W2 — Learning Loop Register (WBS 2.8)

Repository: eduycojr/some_ip_cplus_plus
Branch: ai-assisted-sdlc
Session: S1-W2-WBS2.8-01

Purpose: Convert incorrect or misleading AI outputs observed during WBS 2.1–2.7 work into Learning Loop entries with root-cause category and planned change. Entries are deliberately explicit about evidence and label FACT / INFERRED / PROPOSED.

## Summary
- Total entries (this file): 5 (W2-LL-001 .. W2-LL-005)
- Root-cause categories used: Prompt Ambiguity, Missing Execution Evidence, Incorrect Mapping/Typo, Process Gap, Test Coverage Gap
- Scheduled changes for retest in Increment 2: 2 entries (W2-LL-001, W2-LL-003)

---

### W2-LL-001
- id: W2-LL-001
- title: MessageType mapping typo led to inconsistent error-type guidance
- description: In early requirements and some handover notes there is an inconsistent mapping of error message types (references to 0x02 vs canonical ERR 0x81). This led AI suggestions that referenced the wrong message type (0x02). 
- evidence: docs/ai-sdlc/evidence/W2-2.2_Requirement_Decomposition_Examples.md (lines noting "0x02 was a typo"), docs/ai-sdlc/handovers/WBS-2.6_to_WBS-2.7.md (INFERRED where the mismatch is discussed)
- root-cause: Incorrect Mapping / Typo in requirement artefact (FACT)
- proposed change: Update canonical Requirements doc to explicitly state MessageType::ERR = 0x81 and add a short "Error Codes" table to WORKFLOW_MODEL_v1.md to prevent future ambiguity.
- owner: @eduycojr (PROPOSED)
- scheduled retest: Increment 2 — retest date proposed: 2026-10-26 (PROPOSED / scheduled for Increment 2)
- verification: Updated requirements doc committed + one unit test that asserts MessageType::ERR value used by message_router and test_message_router_error verifies emitted type (evidence: commit & test logs). Status: PROPOSED

---

### W2-LL-002
- id: W2-LL-002
- title: AI claimed tests passed without runner logs
- description: Some AI outputs and handover wording implied tests or coverage "PASS" without attached CI logs or local runner evidence. This violates Evidence-over-Assertion.
- evidence: docs/ai-sdlc/handovers/WBS-2.7_to_WBS-2.8.md (note: "Local execution and coverage reports pending runner invocation") and earlier assistant messages claiming test additions (FACT/INFERRED)
- root-cause: Missing Execution Evidence (PROCESS GAP)
- proposed change: Add mandatory checklist item for handovers: include CI artifact links (Actions run ID, artifact URL) or mark handover as `WAITING_FOR_LOGS` until logs are attached. Update .github/copilot-instructions.md with explicit handover artifact requirements.
- owner: @eduycojr (PROPOSED)
- scheduled retest: not required; procedural change (PROPOSED)
- verification: Handover with attached Actions run links or uploaded logs present in docs/ai-sdlc/evidence/ or linked to workflow artifacts. Status: PROPOSED

---

### W2-LL-003
- id: W2-LL-003
- title: Missing MP1 task→artifact mapping prevents telemetry calculation
- description: Telemetry calculations require a canonical mapping of MP1 tasks to PRs/commits/session IDs and definitions of start/done. This mapping was not present, blocking cycle-time & acceptance-rate measurement.
- evidence: Absence of a machine-readable mapping in docs/ai-sdlc/handovers/ and evidence folder (FACT)
- root-cause: Process Gap (lack of telemetry contract) (FACT)
- proposed change: Create and maintain `docs/ai-sdlc/telemetry/mp1_task_map.yaml` with entries for each MP1 task mapping to PR number(s), commit SHAs, session IDs, start/done definitions, and owner. Make this a required handover artifact for stage transitions.
- owner: @eduycojr (PROPOSED)
- scheduled retest: Increment 2 — task map to be filled and used to compute W2 metrics; retest date proposed: 2026-10-26 (PROPOSED / scheduled for Increment 2)
- verification: Presence of mp1_task_map.yaml and a computed W2 telemetry table with numeric cycle-times in docs/ai-sdlc/evidence/. Status: PROPOSED

---

### W2-LL-004
- id: W2-LL-004
- title: Prompt ambiguity causing missing platform-specific checks (sanitizer/coverage)
- description: AI prompts used during WBS 2.6/2.7 produced CMake additions for coverage and sanitizers but did not mandate local verification steps or platform-conditional instructions, causing inconsistent inclusion of sanitizer flags across platforms.
- evidence: commit f11a5433874420c7f4a4c8bff2eeb5d8fcf5b74c (CMake additions for ENABLE_COVERAGE / ENABLE_SANITIZERS) and WORKFLOW_MODEL_v1.md guidance (FACT)
- root-cause: Prompt Ambiguity / Incomplete Platform Guards (INFERRED)
- proposed change: Update CMake additions to include guard notes and documented build matrix in README (Linux/Clang recommended for sanitizers). Add a checklist item in test handover to attach sanitizer run logs when ENABLE_SANITIZERS=ON.
- owner: @eduycojr (PROPOSED)
- scheduled retest: not scheduled by default (PROPOSED)
- verification: Sanitizer run logs attached to handover evidence. Status: PROPOSED

---

### W2-LL-005
- id: W2-LL-005
- title: Generated tests include environment-dependent assumptions (free port, socket behavior)
- description: Several added tests use a dynamic free-port finder and platform-dependent socket behavior. Without CI job matrix documentation, tests may be flaky on certain OS or runner configurations.
- evidence: tests/test_message_router_error.cpp, tests/test_transport_retry_backoff.cpp, tests/test_transport_stress.cpp (commit f11a5433) (FACT)
- root-cause: Test Coverage Gap / Environment Assumption (FACT)
- proposed change: Harden tests by using deterministic ports via test harness or documenting expected runner environment; add CI job matrix notes and a test wrapper that retries port bind discovery with a controlled fallback and emits verbose logs for failure diagnosis.
- owner: @eduycojr (PROPOSED)
- scheduled retest: not scheduled (PROPOSED)
- verification: CI run on matrix with Ubuntu/Clang and Windows showing deterministic results and attached logs. Status: PROPOSED

---

## Register status and counts (per acceptance criteria)
- Total entries: 5 (meets >=5 cumulative loops) (FACT: this file contains 5 entries)
- Entries with scheduled changes for retest in Increment 2: 2 (W2-LL-001, W2-LL-003) (FACT: scheduled dates are PROPOSED for Increment 2)

---

Generated-by: AI Assistant (Copilot). These entries are intentionally explicit about their PROPOSED status when they require operational changes or execution logs. To convert PROPOSED -> FACT, perform the scheduled actions and attach evidence (commits, PRs, CI artifacts) to docs/ai-sdlc/evidence/ and mark entries as VERIFIED.