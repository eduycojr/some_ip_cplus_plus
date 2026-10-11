# P12 — Documentation and Change Notes

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P12
- **Version:** 1.0.0
- **SDLC stage:** 9 — Documentation and delivery evidence
- **Derived from WBS 1.3 pair(s):** 3, 5, 8
- **Learning Loop tag:** None

## Purpose
Draft concise documentation grounded in verified behavior and the actual change set.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Supply the confirmed behavior, source/diff references, audience, supported version/environment, and known limitations. Do not ask the model to invent commands or behavior.

## Prompt template
```text
Draft documentation for the change using only the supplied verified facts.
Change/diff: {{CHANGE}}
Verified behavior and evidence: {{VERIFIED_FACTS}}
Audience: {{AUDIENCE}}
Version/environment: {{VERSION_ENV}}
Known limitations: {{LIMITATIONS}}

Produce purpose, usage/behavior, example, compatibility notes, verification evidence, and limitations as applicable. Label any missing fact as TBD. Do not claim tests/builds passed unless evidence is supplied. Keep unrelated information out of the change note.
```

## Example output
“Malformed lines without `=` are rejected by the parser in the provided code path. Validation evidence: test output must be attached before this behavior is described as tested.”

## Known limitation
Documentation drafted from partial diffs can omit behavior outside the visible scope; the owning engineer must approve it.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
