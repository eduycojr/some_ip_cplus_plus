# P10 — Code Review with Evidence and Severity

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P10
- **Version:** 1.0.0
- **SDLC stage:** 8 — Review
- **Derived from WBS 1.3 pair(s):** 1, 2, 3
- **Learning Loop tag:** None

## Purpose
Generate actionable review findings tied to changed code, severity, impact, and verification.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Supply the diff plus enough surrounding code, requirement/acceptance criteria, and conventions to evaluate correctness. If no code evidence supports a concern, do not state it as a finding.

## Prompt template
```text
Review this diff as a senior engineer.
Diff: {{DIFF}}
Relevant context/requirements: {{CONTEXT}}
Conventions: {{CONVENTIONS}}

For each finding provide severity, exact file/line or snippet evidence, concrete impact, trigger condition, and a suggested fix or test. Separate correctness defects from maintainability suggestions. Do not report style preference as a defect. If no actionable finding is supported, say so. Finish with unverified assumptions and the checks a human reviewer must perform.
```

## Example output
Finding: Medium — the new branch returns success when the delimiter is absent; evidence: `if (pos == npos)` branch in supplied diff; test: malformed line should return failure.

## Known limitation
AI review can miss defects and generate false positives; it is a review aid, not approval or a substitute for a human code owner.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
