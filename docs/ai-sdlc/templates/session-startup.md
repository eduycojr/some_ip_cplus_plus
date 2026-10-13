# Session Startup Template

Use this file as the startup receipt template for every AI-assisted SDLC session.

Session ID: S[YYYYMMDD]-[WBS]-[NN]
Stage/WBS: [IDs]
Objective: [measurable outcome]
Previous handover: [path + predecessor commit SHA]
Required inputs: [paths + commit SHAs]
Prompt IDs used: [Pxx]

Inspection checklist (record exact paths + SHAs):
- [ ] docs/ai-sdlc/handovers/<previous>.md — commit: <sha>
- [ ] docs/requirements/REQUIREMENTS.md — commit: <sha>
- [ ] src/<relevant files> — commit: <sha>

Context Receipt (required):
1. Objective and acceptance criteria:
   - [text]
2. Files inspected and exact commit SHAs:
   - path: <path> commit: <sha>
3. Confirmed FACTs (with evidence paths):
   - fact — evidence: <path or commit>
4. Inferences / Assumptions:
   - [text]
5. Missing / Blocked items (owner):
   - [text] owner: @username
6. Entry gate result: PASS / BLOCKED (explain)

If entry gate = BLOCKED, stop and list the exact actions and owners required to proceed.

