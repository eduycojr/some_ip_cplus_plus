# P05 — Implementation Plan and Dependency Check

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P05
- **Version:** 1.0.0
- **SDLC stage:** 4 — Planning
- **Derived from WBS 1.3 pair(s):** 6, 10, 11
- **Learning Loop tag:** W1.3-LL-01

## Purpose
Create a sequenced plan that respects dependencies and acceptance constraints, and apply multi-turn planning only when it adds value.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Supply approved requirements, known dependencies, compatibility constraints, existing tests, scope boundary, and estimated delivery constraints if known. Identify which constraints are confirmed vs inferred.

## Prompt template
```text
Create a minimal implementation plan for: {{TASK}}
Acceptance criteria: {{ACCEPTANCE}}
Dependencies and constraints: {{DEPENDENCIES_CONSTRAINTS}}
Existing tests/toolchain: {{TEST_CONTEXT}}

Return ordered steps with dependency, output, verification, and done condition. Then perform a constraint-reconciliation pass: list every supplied constraint and show the step that protects it. Use a single-pass plan unless requirements are materially ambiguous, dependency-dense, or missing important context. If multi-turn planning is justified, state what each turn resolves and the overhead it adds. Do not invent repository paths or commands.
```

## Example output
Plan includes locating call path, implementing the smallest change, adding boundary tests, running existing tests, inspecting diff, and linking evidence to each acceptance criterion.

## Known limitation
Estimates and sequencing are provisional until confirmed against the actual repository and toolchain. This prompt does not itself execute steps.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
