# P15 — Evidence-First Technical Claim Verification

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P15
- **Version:** 1.0.0
- **SDLC stage:** 2 — Context and repository discovery; 8 — Review
- **Derived from WBS 1.3 pair(s):** 8, 9
- **Learning Loop tag:** None

## Purpose
Check a technical claim against supplied source evidence and explicitly separate verified, contradicted, and unknown claims.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Supply the claim, exact evidence excerpts or tool output, and source identifiers/paths. Do not imply that unprovided documentation was inspected.

## Prompt template
```text
Evaluate this claim using only the evidence attached.
Claim: {{CLAIM}}
Evidence with source path/identifier: {{EVIDENCE}}

Choose one: SUPPORTED, CONTRADICTED, or UNKNOWN. Quote the smallest relevant evidence excerpt and explain the logical connection. List what the evidence does not establish and the next source/test needed. Never fabricate a source, API name, file path, symbol, build result, or test result. If the evidence is incomplete, choose UNKNOWN.
```

## Example output
Claim: “The function returns false for a line without `=`.” Result: SUPPORTED by the supplied branch `if (pos == npos) return false;`. Unknown: whether the caller displays a particular error message.

## Known limitation
Supplied snippets may be stale or omit relevant caller behavior; source version and context must be confirmed.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
