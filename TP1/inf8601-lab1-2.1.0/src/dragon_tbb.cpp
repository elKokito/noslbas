/*
 * dragon_tbb.c
 *
 *  Created on: 2011-08-17
 *      Author: Francis Giraldeau <francis.giraldeau@gmail.com>
 */

#include <iostream>

extern "C" {
#include "dragon.h"
#include "color.h"
#include "utils.h"
}
#include "dragon_tbb.h"
#include "tbb/tbb.h"

using namespace std;
using namespace tbb;

class ResourceManager
{

private:

	ResourceManager() {}

public:

	~ResourceManager() {}

	// Lightweight singleton implementation
	static ResourceManager& Instance()
	{
		static ResourceManager s_rmInstance;
		return s_rmInstance;
	}

	void* AddEntry(const size_t a_unDataSize)
	{
	    cout << "Adding an entry of " << a_unDataSize << " bytes in resource manager." << endl;
        void* pData = malloc(a_unDataSize);
		m_vResourcePool.push_back(pData);
		return pData;
	}

	void ReleaseResources()
	{
	    cout << "Releasing " << m_vResourcePool.size() << " allocated entries from resource manager." << endl;
		for (void* resource : m_vResourcePool)
		{
			free(resource);
		}
		m_vResourcePool.clear();
	}

private:
	vector<void*> m_vResourcePool;

};

class DragonLimits {
    
public:

    DragonLimits(tbb::concurrent_vector<struct limit_data>* pieces) 
    {
        m_master = (piece_t*)ResourceManager::Instance().AddEntry(sizeof(piece_t));
        m_pieces = pieces;
        piece_init(m_master);
    }

    piece_t getPiece()
    {
        // Sorting needed here, since we don't
        // want to merge pieces without ordering them
        tbb::parallel_sort(
            m_pieces->begin(), 
            m_pieces->end(),
            &compareByStart);

        for (size_t i = 0; i < m_pieces->size(); ++i)
        {
            piece_merge(m_master, (*m_pieces)[i].piece);
        }
        return *m_master;
    }

    void operator() (const tbb::blocked_range<uint64_t>& range) const
    {
        struct limit_data thread_data;
        piece_init(&thread_data.piece);
        thread_data.start = range.begin();
        thread_data.end = range.end();
        piece_limit(
            thread_data.start,
            thread_data.end, 
            &thread_data.piece);
        m_pieces->push_back(thread_data);
   } 

private:

    static bool compareByStart( const struct limit_data& data1, const struct limit_data& data2)
    {
        return data1.start < data2.start;
    }

    piece_t                                   *m_master;
    tbb::concurrent_vector<struct limit_data> *m_pieces;

};

class DragonDraw {

public:
  
    DragonDraw(struct draw_data* data)
        : m_data(data), m_color(0)
    {}
    
    void setColor(const int color)
    {
        m_color = color;
    }
    
    void operator() (const tbb::blocked_range<uint64_t>& range) const
    {
        dragon_draw_raw(
            range.begin(),
            range.end(),
            m_data->dragon,
            m_data->dragon_width,
            m_data->dragon_height,
            m_data->limits,
            m_color);
    }

private:

   struct draw_data *m_data;
    int m_color;
};

class DragonRender {

public:
  
    DragonRender(struct draw_data* data)
        : m_data(data)
    {}
    
    void operator() (const tbb::blocked_range<int>& range) const
    {
        scale_dragon(
                range.begin(),
                range.end(),
                m_data->image,
                m_data->image_width,
                m_data->image_height,
                m_data->dragon,
                m_data->dragon_width,
                m_data->dragon_height,
                m_data->palette);
    }

private:

   struct draw_data *m_data;

};

class DragonClear {

public:
  
    DragonClear(struct draw_data* data)
        : m_data(data)
    {}
    
    void operator() (const tbb::blocked_range<int>& range) const
    {
        init_canvas(
            range.begin(),
            range.end(),
            m_data->dragon,
            -1);
    }

private:

   struct draw_data *m_data;

};

int dragon_draw_tbb(char **canvas, struct rgb *image, int width, int height, uint64_t size, int nb_thread)
{
	struct draw_data data;
	limits_t limits;
	char *dragon = NULL;
	int dragon_width;
	int dragon_height;
	int dragon_surface;
	int scale_x;
	int scale_y;
	int scale;
	int deltaJ;
	int deltaI;

	struct palette *palette = init_palette(nb_thread);
	if (palette == NULL)
		return -1;

	/* 1. Calculer les limites du dragon */
	dragon_limits_tbb(&limits, size, nb_thread);

	dragon_width = limits.maximums.x - limits.minimums.x;
	dragon_height = limits.maximums.y - limits.minimums.y;
	dragon_surface = dragon_width * dragon_height;
	scale_x = dragon_width / width + 1;
	scale_y = dragon_height / height + 1;
	scale = (scale_x > scale_y ? scale_x : scale_y);
	deltaJ = (scale * width - dragon_width) / 2;
	deltaI = (scale * height - dragon_height) / 2;

	dragon = (char *) malloc(dragon_surface);
	if (dragon == NULL) {
		free_palette(palette);
		*canvas = NULL;
		return -1;
	}

	data.nb_thread = nb_thread;
	data.dragon = dragon;
	data.image = image;
	data.size = size;
	data.image_height = height;
	data.image_width = width;
	data.dragon_width = dragon_width;
	data.dragon_height = dragon_height;
	data.limits = limits;
	data.scale = scale;
	data.deltaI = deltaI;
	data.deltaJ = deltaJ;
	data.palette = palette;

	task_scheduler_init init(nb_thread);

	/* 2. Initialiser la surface : DragonClear */
    DragonClear clear(&data);
    tbb::parallel_for( tbb::blocked_range<int>(0, dragon_surface), clear);
    /* 3. Dessiner le dragon : DragonDraw */
    DragonDraw  draw(&data);
    for (int i = 0; i < nb_thread; ++i)
    {
		uint64_t start_color = i * size / nb_thread;
		uint64_t end_color = (i + 1) * size / nb_thread;
        draw.setColor(i);
        tbb::parallel_for( tbb::blocked_range<uint64_t>(start_color, end_color), draw);
    }
	/* 4. Effectuer le rendu final : DragonRender */
    DragonRender render(&data);
    tbb::parallel_for( tbb::blocked_range<int>(0, height), render);

	init.terminate();
	free_palette(palette);
    *canvas = dragon; // FIXME: retourner le dragon calculé
    ResourceManager::Instance().ReleaseResources();
	return 0;
}

/*
 * Calcule les limites en terme de largeur et de hauteur de
 * la forme du dragon. Requis pour allouer la matrice de dessin.
 */
int dragon_limits_tbb(limits_t *limits, uint64_t size, int nb_thread)
{
    cout << "Started dragon_limits_tbb" << endl;
    tbb::concurrent_vector<struct limit_data> pieces;
    DragonLimits lim(&pieces);

    task_scheduler_init init(nb_thread);
    tbb::parallel_for(tbb::blocked_range<uint64_t>(0,size), lim);
    init.terminate();

    piece_t piece = lim.getPiece();
	*limits = piece.limits;
    cout << "Finished dragon_limits_tbb" << endl;
    return 0;
}
