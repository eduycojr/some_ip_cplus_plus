# P13 — Refactoring Safety and Scope Guard

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P13
- **Version:** 1.0.0
- **SDLC stage:** 10 — Maintenance and controlled change
- **Derived from WBS 1.3 pair(s):** 5, 10
- **Learning Loop tag:** None

## Purpose
Evaluate whether a proposed refactor is necessary, bounded, and protected by appropriate regression checks.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide the proposed refactor diff, reason, behavior that must remain unchanged, existing tests, and explicitly permitted scope.

## Prompt template
```text
Assess this refactor proposal against the stated goal and scope.
Goal: {{GOAL}}
Diff/proposed changes: {{DIFF}}
Behavior to preserve: {{PRESERVED_BEHAVIOR}}
Existing tests: {{TESTS}}
Permitted scope: {{SCOPE}}

Identify required changes versus unrelated changes, compatibility risks, public-interface changes, missing tests, and a minimal revert boundary. Recommend proceeding only where evidence supports it. Provide a before/after behavior checklist. Do not broaden the patch just for style preference.
```

## Example output
The helper rename is unrelated to the parsing defect and should be removed from the patch; regression checks must cover valid input, missing delimiter, and preserved output behavior.

## Known limitation
A static review cannot prove behavioral equivalence; tests and human diff review are still required.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
