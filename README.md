# SledProc-plus-ChasmVM (introduction)

Since roughly 2016, I have worked on a series of 
projects for a startup company called 
Linguistic Technology Systems, based in 
New Jersey.  Our initial focus 
was on native UI development, particularly 
for applications based on Qt.  Our premise was 
that many organizations were switching completely 
to web-based portals (which is different from 
providing web access as one user option in a 
hybrid application that also can run as a 
standalone desktop program) -- and that many 
applications suffered from degradation in quality 
and usability as a result.  (It's worth noting 
that many projects seem to be built as web apps 
even when intended to be run locally on one machine, 
essentially launching a separate HTTP server solely 
to support a single program's UI.)  We believed 
that improving the development workflow for native 
C++ software -- e.g., a wider range of prebuilt 
components for UI domains such as annotated video players, 
digital maps, PDF viewers, managing Image Processing piplines, 
etc.; plus embeddable and self-contained scripting, database, 
and cloud-integration capabilities -- and also offering 
embedded web viewers for content that does work best 
in a browser context (such as interactive SVG, or 360 photography 
via WebGL) would help developers transition from browser-based 
paradigms to a native and/or hybrid model, with better 
overall UX and responsiveness.

From that admittedly ambitious starting point, we have 
gradually refined our focus to the specific field 
of Data Publishing.  To describe this concern as a 
domain in itself acknowledges a distinction between 
merely sharing raw data files, and a more modern 
understanding of Data Publishing in the context of 
Open Science, transparency, and research replication.  In 
academic contexts, Data Publishing is encouraged 
(at least in theory) not only to diminish the 
likelihood of inadvertant processing or analysis 
errors (not to mention outright fabrication) but also 
to pave the way for future research extending, regenerating, 
and/or reanalyzing published data sets.  Authors should 
prioritize not only allowing outside parties to 
_access_ research data, but also to reuse it productively.

Such a perspective on Data Publishing puts an emphasis on 
the software that will be used to read, examine, and 
visualize data sets.  Raw data files might be loaded 
into prexisting applications (depending on their format) 
but relying on expensive or proprietary software (and 
hardware) is suboptimal; as much as possible, instead, 
published data sets should include code or facilities 
to streamline their usability via readily available, 
open-source components.  This might entail ensuring 
that maintained and up-to-date code libraries exist 
to support parsing the file type(s) present in a data 
set (sensitive to versioning and compatibility issues). 
Even better, supporting code to make raw files 
at least provisionally usable is ideally included 
as part of a unified data package.  Data Publications 
in this sense can incorporate source files for 
standalone applications through which researchers 
examine, manage, and visualize shared data.

This use-case is a special example of the more general 
goal of facilitating native GUI programming.  In 
order for a standalone native application to 
explicitly support one or more individual data sets, 
distributed as part of an overarching publication, 
users should be able to compile the included code 
with minimal external dependencies.  For scientific 
(and many other) projects, GUIs should nonetheless have 
robust capabilities, consistent with data sets 
whose content includes audio, video, GIS maps, 
information profiles mixing tabular and hierarchical 
structures (thereby needing special GUI displays), 
and so forth.  This implies a code base that supports 
a variety of media types and special-purpose GUI 
components, while still being distributable in 
source code fashion.

Because this is all a work in progress, the components 
themselves are admittedly rather unpolished and 
dubiously documented.  Eventually, I hope to refine 
the respective source-file groups ("modules", essentially) 
so that they're easy to use just dropped in to 
C++ projects, and also could be extended to support 
the needs of individual applications.  I realize that 
sharing basically unfinished code is unprofessional 
in many contexts, but I ask anyone reading this to 
make allowances for our situation: I'm the only 
programmer working on these projects, and we're 
splitting time between intensive coding and 
other tasks, such as writing academic texts 
and preparing for meetings and other "marketing" situations.
When discussing Data Publishing software, it is easy 
for the conversation to become quite vague and 
abstract without having some concrete code to 
refer to, even if that code still exists in a 
prototype or proof-of-concept stage.

During the early stages of our project focused more 
broadly on GUI (and native-application) development, 
we prototyped a simple scripting language 
engineered for easy interoperability with Qt.  Our 
reasoning was that scripting capabilities 
(alongside database persistence, multimedia, etc.) 
should be handled by modules that could be 
distributed in source code fashion and examined 
transparently in a C++ development environment, 
such as the Qt Creator IDE.  Even scripting 
languages that integrate well with C++ 
can be at least partially opaque behind the 
scenes: AngelScript, for example, which can be 
included into larger projects in source code 
fashion, drops into assembly for FFI logic 
and so certain stages in script's evaluation 
cannot really be observed first-hand from a 
debugger/IDE.  Mainstream languages like 
Python or JavaScript don't appear to have 
implementations that can be built straightforwardly 
from source (Embedded Common Lisp -- ECL -- is a 
viable alternative, but there is an extra 
level of adjustment for C++ developers coding in Lisp in the 
first place).  

Although we believe these remain valid reasons why a 
Qt-based scripting language would have legitimate 
use-cases, focusing specifically on Data Publishing 
points toward further contexts suitable 
for customized scripting.  There are many 
nonstandard file formats specific to scientific 
disciplines that require specialized parsing 
libraries -- moreover, even with generic 
serialization such as via XML or JSON, reading a 
data set will normally require further processing 
or traversal after a full XML (say) document 
and infoset is loaded.  FAIRSharing access 
to open-source data sets can therefore depend 
on the usability of deserialization libaries 
that map raw data files onto live application 
objects.  We feel this situation represents a 
further rationale for a scripting environment 
optimically focused on Data Publishing.  
Libraries for parsing and processing data 
files in discipline-specific formats could 
be compiled to a specialized embeddable 
Virtual Machine which is lightweight enough 
to run within many science-related applications 
and to be compiled in a cross-platform manner.  
Up-to-date deserialization libaries could therefore 
be maintained by targeting at least this 
Data Publishing VM, even if there is a lag 
in updating code for different platforms 
and environments insofar as data formats evolve.

We have therefore designed SledProc (Software 
Language Engineering for Data Publishing -- 
Prototype Compiler/Runtime) as a special-purpose 
scripting language for the Data Publishing problem 
case.  Alongside SledProc is ChasmVM ("Channel/Syntagm 
Virtual Machine"; see references for an explanation 
for this terminology) that is operationally 
separate in that alternative source-level formats 
could potentially compile to ChasmVM code.  
We believe the use-cases within Data Publishing -- especially 
in the broad sense outlined here, that overlaps 
with concerns such as native GUI development -- 
are sufficiently distinct that a specialized  
scripting environment may offer concrete benefits, 
justifying the effort to create yet another 
scripting language.  SledProc does not aim for 
optimizations or for general-purposes use-cases 
in the sense of Python, or JavaScript, etc.  
On the other hand, SledProc is desinged to be 
somewhat larger than just as Data Publishing 
DSL; it supports general-purpose programming 
particularly in the context of analyzing 
and working with data sets, and is optimized 
for the precision and pedagogical requirements 
of scientific data -- utilizing constructs 
such as explicit SI units, dimensional analysis, 
queryable code libraries, and other 
features through which the code supporting 
a data set can help illustrate the scientific 
theory and experimental protocols through which 
published data was acquired.

In the SledProc/ChasmVM context, a "module" 
is actually a group of C++ files 
providing functionality that could be 
invoked from SledProc code (and, in general, 
embed a SledProc compiler and ChasmVM runtime 
allowing for calls in the opposite direction 
as well).  Our GUI components (and a few 
others, such as tools for Natural Language 
documents' text encoding) can accoringly 
be developed as SledProc modules, 
which the form in general that they will 
take in this repository (although 
users may find reasons to incorporate 
some of the C++ code without finding use-cases 
for the overall SledProc project).  
  

 
