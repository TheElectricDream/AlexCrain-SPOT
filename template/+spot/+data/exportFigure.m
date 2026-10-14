function file = exportFigure(fig, file, opts)
%EXPORTFIGURE Save a figure for a paper or thesis, as the lab's GetStarted template does.
%   spot.data.exportFigure(fig, file) sets the "paper" to the figure's size in inches (width x
%   height, default 7 x 4), uses the vector renderer, and prints at 700 dpi. The format follows
%   the extension: .pdf (default), .eps, .svg or .png.
%   spot.data.exportFigure(fig, file, opts) with opts.width and opts.height [in].
%   (Templates/graph_print_example.m in github.com/Carleton-SRL/GetStarted)
if nargin < 3
    opts = struct();
end
[folder, ~, ext] = fileparts(file);
if isempty(ext)
    ext = '.pdf';
    file = [file ext];
end
if ~isempty(folder) && ~isfolder(folder)
    mkdir(folder);
end
set(fig, 'Units', 'inches');
pos = get(fig, 'Position');
w = pos(3); h = pos(4);
if isfield(opts, 'width') && ~isempty(opts.width), w = opts.width; end
if isfield(opts, 'height') && ~isempty(opts.height), h = opts.height; end
set(fig, 'Position', [pos(1) pos(2) w h]);
set(fig, 'PaperUnits', 'inches', 'PaperPosition', [0 0 w h], 'PaperSize', [w h]);
switch lower(ext)
    case '.png'
        print(fig, file, '-dpng', '-r700');
    case '.eps'
        set(fig, 'Renderer', 'painters');
        print(fig, file, '-depsc', '-r700');
    case '.svg'
        set(fig, 'Renderer', 'painters');
        print(fig, file, '-dsvg', '-r700');
    otherwise
        set(fig, 'Renderer', 'painters');
        print(fig, file, '-dpdf', '-r700');
end
end
