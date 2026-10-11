# P03 — Repository Exploration with Evidence Boundaries

**SIMULATION ARTIFACT — WBS 1.4 Engineering prompt library v1**

- **Prompt ID:** P03
- **Version:** 1.0.0
- **SDLC stage:** 2 — Context and repository discovery
- **Derived from WBS 1.3 pair(s):** 8, 9
- **Learning Loop tag:** None

## Purpose
Summarize repository structure and trace a behavior from actual supplied files while separating verified facts from hypotheses.

## When to use
Use this prompt when the task matches the stated purpose and the required input contract can be satisfied. If required evidence is missing, request it or return an explicit unknown/blocked result rather than fabricate it.

## Input contract
Supply relevant file paths and excerpts or an actual repository listing. State the scope searched. Do not ask the model to claim it inspected files that were not provided. Exclude credentials and company-confidential data.

## Prompt template
```text
Explore only the repository evidence supplied below.
Scope searched: {{SCOPE}}
File tree or file paths: {{FILE_LIST}}
Relevant source excerpts: {{SOURCE_EXCERPTS}}
Task question: {{QUESTION}}

Return: (1) verified facts with file path/symbol evidence, (2) inferred hypotheses clearly marked, (3) unknowns, (4) next files or symbols to inspect, and (5) a minimal verification plan. Never invent filenames, symbols, API behavior, test results, or repository configuration. If supplied evidence is insufficient, say what is missing.
```

## Example output
Verified: `parse_pair` searches for `=` and returns false when it is absent (provided `parser.cpp` excerpt). Unknown: whether a higher-level caller converts false into a user-facing error.

## Known limitation
The prompt cannot verify files outside supplied context or a real connected repository. File excerpts can be incomplete or stale.

## Human verification checklist
- [ ] Required input fields are present and safe to share.
- [ ] Claims and outputs are supported by supplied evidence.
- [ ] Unknowns and assumptions are explicitly labelled.
- [ ] Acceptance criteria are mapped to observable checks where relevant.
- [ ] Any code/schema/test result is independently verified before reuse.
- [ ] Human reviewer decision is recorded separately from AI output.
