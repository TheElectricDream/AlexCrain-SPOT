function file = exportFigure(fig, file, opts)
    %EXPORTFIGURE Save a figure for a paper or thesis, as the lab's GetStarted template does.
    %   file = spot.data.exportFigure(fig, file) sets the paper to the figure's size, uses the
    %   vector renderer, and prints at 700 dpi. The format follows the extension: .pdf
    %   (default), .eps, .svg or .png.
    %   file = spot.data.exportFigure(fig, file, opts) sets the size.
    %
    %   Inputs
    %     fig   figure handle
    %     file  output file; .pdf is added if there is no extension
    %     opts  optional width and height [in] (default: the figure's size)
    %
    %   Outputs
    %     file  the file written
    %
    %   Based on Templates/graph_print_example.m in github.com/Carleton-SRL/GetStarted.

    %% Defaults
    if nargin < 3
        opts = struct();
    end

    %% Constants
    RESOLUTION = '-r700';   % 700 dpi

    %% File
    % Default extension, and the folder.
    [folder, ~, ext] = fileparts(file);
    if isempty(ext)
        ext = '.pdf';
        file = [file ext];
    end
    if ~isempty(folder) && ~isfolder(folder)
        mkdir(folder);
    end

    %% Size
    % The figure's size in inches, unless given.
    set(fig, 'Units', 'inches');
    pos = get(fig, 'Position');
    width = pos(3);
    height = pos(4);
    if isfield(opts, 'width') && ~isempty(opts.width)
        width = opts.width;
    end
    if isfield(opts, 'height') && ~isempty(opts.height)
        height = opts.height;
    end

    % Figure and paper the same size.
    set(fig, 'Position', [pos(1) pos(2) width height]);
    set(fig, 'PaperUnits', 'inches', 'PaperPosition', [0 0 width height], ...
        'PaperSize', [width height]);

    %% Print
    % Vector formats use the painters renderer.
    switch lower(ext)
        case '.png'
            print(fig, file, '-dpng', RESOLUTION);
        case '.eps'
            set(fig, 'Renderer', 'painters');
            print(fig, file, '-depsc', RESOLUTION);
        case '.svg'
            set(fig, 'Renderer', 'painters');
            print(fig, file, '-dsvg', RESOLUTION);
        otherwise
            set(fig, 'Renderer', 'painters');
            print(fig, file, '-dpdf', RESOLUTION);
    end
end
