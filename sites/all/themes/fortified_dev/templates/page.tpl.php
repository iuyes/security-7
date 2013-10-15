<!-- Header and Nav -->
<nav class="top-bar">
  <ul class="title-area">
    <li class="name"><h3 class="site-title"><a href='http://www.fortifiedsecurityservices.com'><img src='http://www.fortifiedsecurityservices.com/sites/default/files/fort_vector.png' width="250px" height="80px" class="site-logo"></a><?php print '<div class="site-name">'.$linked_site_name.'</div><span class="pone"> | (520) 886-6419</span>'; ?></h3></li>
    <li class="toggle-topbar menu-icon"><a href="#"><span></span></a></li>
  </ul>
  
  <div class="top-bar-section">
    <?php if ($base_menu_links) :?>
    	<div class="default-menu">
      	<?php print $base_menu_links; ?>
    	</div>
    <?php endif; ?>
     <div class="main-menu-links">
    	<?php print $main_menu_links; ?>
    </div>
  </div>
  <div class="top-bar-search"> 
  	<?php echo render($search_form); ?>
	</div>
</nav>
	
<div class="middle-head-menu">
	<div class="main-menu-wrapper">
    <?php if (!empty($page['navigation'])): ?>
      <?php print render($page['navigation']);?>
    <?php endif; ?>
  </div>
</div>

<?php if ($breadcrumb): ?>
<!---Breadcrumbs Header-->
<div class="large-8 large-centered columns">
  <div class="header-bottom">
  	<?php print $breadcrumb; ?>
  </div>
</div>
<!--- END Breadcrumbs Header-->
<?php endif; ?>

<!---Highlighted-->
<?php if (!empty($page['highlighted'])): ?>
	<div class="hihglighted wrapper row">
		<div class="large-12 large-centered columns">
			<div class="highlight panel callout">
				<?php print render($page['highlighted']); ?>
			</div>
		</div>
	</div>
<?php endif; ?>
<!--- End Highlighted-->

<!---Slide Show--->
<div class="slideshow">
	<div class="slide-show">
		<?php print render($page['slide_show']); ?>
	</div>
</div>
<!---END Slide Show--->

<div class="main-page-content row"><!--#row -->
	<?php if ($messages): print $messages; endif; ?>
  <?php if (!empty($page['help'])): print render($page['help']); endif; ?>
  	<div id="main" class="large-12 columns">
    <?php if ($title && !$is_front): ?>
      <?php print render($title_prefix); ?>
      <h1 id="page-title" class="title"><?php print $title; ?></h1>
      <?php print render($title_suffix); ?>
    <?php endif; ?>
      <?php if (!empty($tabs)): ?>
        <?php print render($tabs); ?>
        <?php if (!empty($tabs2)): print render($tabs2); endif; ?>
      <?php endif; ?>
      
      <?php if ($action_links): ?>
        <ul class="action-links">
          <?php print render($action_links); ?>
        </ul>
      <?php endif; ?>
    <?php print render($page['content']); ?>
    </div> <!--/ #main-->
</div><!--/.row -->

<?php if (!empty($page['footer_first']) || !empty($page['bottom_menu']) || !empty($page['footer_last'])): ?>
<footer class="footer">
		<div class="specials">
	    <?php if (!empty($page['footer_first'])): ?>
	      <div id="footer-first" class="row">
	        <?php print render($page['footer_first']); ?>
	      </div>
	    <?php endif; ?>
		</div>
		<div class="bottom-bar">
	    <div class="footer-bottom">
	      <?php if(!empty($page['bottom_menu'])) :?>
	        <?php print render($page['bottom_menu']); ?>
	      <?php endif; ?>
	    <?php if (!empty($page['footer_last'])): ?>
	      <div id="footer-last" class="row">
	        <?php print render($page['footer_last']); ?>
	      </div>
	    <?php endif; ?>
	    </div>
	  </div>
</footer>
<?php endif; ?>
    <div class="copyright large-12 columns">
      <?php if ($site_name) :?>
        &copy; <?php print date('Y') . ' ' . check_plain($site_name) . ' ' . t('All rights reserved.'); ?>
      <?php endif; ?>
    </div>
<div id="modal">
  <?php print render($page['modal']); ?>
</div>
<?php  drupal_add_js('var _gaq=[["_setAccount","UA-41151999-1"],["_trackPageview"]];(function(d,t){var g=d.createElement(t),s=d.getElementsByTagName(t)[0];g.async=1; g.src=("https:"==location.protocol?"//ssl":"//www")+".google-analytics.com/ga.js"; s.parentNode.insertBefore(g,s)}   (document,"script"));'
, array('type' => 'inline', 'scope' => 'footer', 'weight' => 2));   ?>