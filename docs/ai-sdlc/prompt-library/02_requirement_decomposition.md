# P02 — Requirement Decomposition and Traceability

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P02
- **Version:** 1.0.0
- **SDLC stage:** 1 — Requirements and acceptance; 4 — Planning
- **Derived from WBS 1.3 pair(s):** 3, 6, 10
- **Learning Loop tag:** None

## Purpose
Convert an approved requirement into a small implementation/test/evidence task tree with explicit dependencies and completion checks.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Provide a requirement ID, approved requirement text, existing architecture constraints, test framework if known, and delivery boundary. If any item is absent, note the gap; do not fabricate file names or components.

## Prompt template
```text
Decompose the approved requirement below into the smallest reviewable engineering tasks.
Requirement ID: {{REQUIREMENT_ID}}
Requirement: {{REQUIREMENT}}
Constraints: {{CONSTRAINTS}}
Known repository/test context: {{CONTEXT}}

Return a table with Task ID, Action, Dependency, Expected artifact, Verification/check, Acceptance link, Risk/unknown. Include implementation, tests, verification, documentation if impacted, and evidence capture. Keep the change within the stated scope. Flag tasks blocked by missing information. End with a coverage check mapping every acceptance criterion to at least one task and one verification step.
```

## Example output
| T-01 | Find parser entry point | None | call-path note | cite source location | AC-01 | target function unknown until repo inspection |

## Known limitation
The plan may be incomplete until repository discovery confirms real modules, build steps, and dependencies.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
