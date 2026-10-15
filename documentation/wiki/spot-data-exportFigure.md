# spot.data.exportFigure

**File:** `+spot/+data/exportFigure.m` · **Package:** [[spot.data|spot-data]]

<!-- source: +spot/+data/exportFigure.m -->

## What it is for

Save a figure the way the lab's GetStarted `graph_print_example.m` does: the paper set to the figure's size in inches, the vector renderer, 700 dpi; the format follows the extension (`.pdf` by default, `.eps`, `.svg`, `.png`).

## How to call it

```matlab
spot.data.exportFigure(fig, 'Saved Data/run/figure.pdf');
spot.data.exportFigure(fig, 'figure.png', struct('width', 7, 'height', 4));
```

## Line by line

**Lines 8–15.** Options; split the path; no extension means `.pdf`.

**Lines 16–18.** Make the folder if needed.

**Lines 19–25.** The figure's size in inches, overridden by the options; set the paper to exactly that size and position.

**Lines 26–38.** Print: PNG as a bitmap at 700 dpi; EPS, SVG and PDF with the `painters` (vector) renderer.

<!-- xref -->
- **Used by:** `+spot/+data/toFigure.m`, `+spot/+worker/Worker.m`
- **Uses:** no other spot function
<!-- /xref -->

## The code
<!-- listing -->
<details>
<summary><b>+spot/+data/exportFigure.m</b> (39 lines; click to show)</summary>

```matlab
   1  function file = exportFigure(fig, file, opts)
   2  %EXPORTFIGURE Save a figure for a paper or thesis, as the lab's GetStarted template does.
   3  %   spot.data.exportFigure(fig, file) sets the "paper" to the figure's size in inches (width x
   4  %   height, default 7 x 4), uses the vector renderer, and prints at 700 dpi. The format follows
   5  %   the extension: .pdf (default), .eps, .svg or .png.
   6  %   spot.data.exportFigure(fig, file, opts) with opts.width and opts.height [in].
   7  %   (Templates/graph_print_example.m in github.com/Carleton-SRL/GetStarted)
   8  if nargin < 3
   9      opts = struct();
  10  end
  11  [folder, ~, ext] = fileparts(file);
  12  if isempty(ext)
  13      ext = '.pdf';
  14      file = [file ext];
  15  end
  16  if ~isempty(folder) && ~isfolder(folder)
  17      mkdir(folder);
  18  end
  19  set(fig, 'Units', 'inches');
  20  pos = get(fig, 'Position');
  21  w = pos(3); h = pos(4);
  22  if isfield(opts, 'width') && ~isempty(opts.width), w = opts.width; end
  23  if isfield(opts, 'height') && ~isempty(opts.height), h = opts.height; end
  24  set(fig, 'Position', [pos(1) pos(2) w h]);
  25  set(fig, 'PaperUnits', 'inches', 'PaperPosition', [0 0 w h], 'PaperSize', [w h]);
  26  switch lower(ext)
  27      case '.png'
  28          print(fig, file, '-dpng', '-r700');
  29      case '.eps'
  30          set(fig, 'Renderer', 'painters');
  31          print(fig, file, '-depsc', '-r700');
  32      case '.svg'
  33          set(fig, 'Renderer', 'painters');
  34          print(fig, file, '-dsvg', '-r700');
  35      otherwise
  36          set(fig, 'Renderer', 'painters');
  37          print(fig, file, '-dpdf', '-r700');
  38  end
  39  end
```

</details>
<!-- /listing -->
